from serial import Serial, to_bytes
from .helpers import crc8
from threading import RLock
import ctypes
import time

SYNC_BYTE = 0x55
WRITE_BYTE = 128
READ_BYTE = 0x00

serial = None
lock = RLock()

def serial_init(port = "/dev/serial0", baud = 1000000):
    global serial
    try:
        if serial is not None and serial.isOpen():
            serial.close()
        print("Starting Serial on port: " + port + " with baud: "+ baud)
        serial = Serial(port, baud, timeout=1, write_timeout=1,inter_byte_timeout=1)
        serial.reset_input_buffer()
        print("Serial connected using: " + serial.name)
        return True
    except Exception as error:
        print("Unable to open serial: "+str(error))
        return False
def serial_recieve():
    global serial
    # parses a packet from the serial buffer, returns cmd, data
    if (serial == None):
        return None

    # check if there is data in the buffer
    if (serial.in_waiting < 1):
        return None

    # wait for the start byte
    try:
        start = None
        while start != SYNC_BYTE:
            start = serial.read(1)[0]
            if start is None:
                print("No start byte found")
                return None
    except Exception as err:
        print("Exception waiting for start byte: " + str(err))
        return None

    # read the command byte
    cmd = None
    try:
        cmd = serial.read(1)[0] & 0x7F
        if cmd is None:
            print("No command byte found")
            return None
    except Exception as err:
        print("Exception reading command byte: " + str(err))
        return None

    # read the data length
    length = None
    try:
        length = ctypes.c_uint16.from_buffer_copy(serial.read(2)).value
        if length is None:
            print("No length byte found")
            return None
    except Exception as err:
        print("Exception reading length byte: " + str(err))
        return None

    # read the data
    data = None
    try:
        data = serial.read(length)
        if data is None:
            print("No data found")
            return None
        if len(data) != length:
            print("Invalid data length")
            return None
    except Exception as err:
        print("Exception reading data: " + str(err))
        return None

    # read the crc
    crc = None
    try:
        crc = serial.read(1)[0]
        if crc is None:
            print("No crc found")
            return None
    except Exception as err:
        print("Exception reading crc: " + str(err))
        return None

    # check the crc
    crc_calc = crc8(data,length)
    if crc_calc != crc:
        print(f'Legnth: {length}')
        print(f'Command: {cmd}')
        print(f'Data: {data}')
        print(f'Invalid CRC {crc_calc} != {crc}')
        return None

    return cmd, data, length


def send(cmd, data):
    global serial
    global lock
    with lock:
        if (serial == None):
            print("Serial is not initialized")
            return
        try:
            serial.write(bytearray([SYNC_BYTE, cmd]))
            if (data != None):
                byte_data = bytes(data, 'utf-8') + b'\x00'
                byte_size = len(byte_data).to_bytes(2, 'little')
                byte_crc = crc8(byte_data, len(byte_data)).to_bytes(2, 'little')
                print("Sending: " ,byte_data , " bytes")
                print("Size: ", byte_size)
                print("CRC: ", byte_crc)
                serial.write(byte_size)
                serial.write(byte_data)
                serial.write(byte_crc)
        except Exception as error:
            print("Exception while sending write command: " + str(error))

def serial_read(cmd, data = None):
    # Sends a read command with optional data
    send(cmd | READ_BYTE, data)

def serial_write(cmd, data):
    # Sends a write command with data
    send(cmd | WRITE_BYTE, data)

def serial_test():
    send(0x55, None)