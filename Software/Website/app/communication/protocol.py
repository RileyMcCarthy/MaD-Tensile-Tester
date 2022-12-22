from serial import Serial
from .helpers import crc8
from threading import RLock

SYNC_BYTE = 0x55
WRITE_BYTE = 128
READ_BYTE = 0x00

commands = None
serial = None
lock = RLock()

def serial_init(cmds, port = "/dev/serial0", baud = 1000000):
    global commands
    global serial
    commands = cmds
    try:
        if serial is not None and serial.isOpen():
            serial.close()

        print("Starting Serial on port: " + port)
        serial = Serial(port, baud, timeout=1, write_timeout=1)
        serial.reset_input_buffer()

    except Exception as error:
        print("Unable to open serial: "+str(error))
        return False
    
    print("Serial connected using: " + serial.name)
    return True

def serial_recieve():
    global serial
    # parses a packet from the serial buffer, returns cmd, data
    if (serial == None):
        print("Serial is not initialized")
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

    # check if the command is valid
    if cmd not in commands:
        print("Invalid command byte: " + str(cmd))
        return None

    # read the data length
    length = None
    try:
        length = serial.read(1)[0]
        if length is None:
            print("No length byte found")
            return False
    except Exception as err:
        print("Exception reading length byte: " + str(err))
        return False

    # read the data
    data = None
    try:
        data = serial.read(length)
        if data is None:
            print("No data found")
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
    if crc8(data) != crc:
        print("Invalid CRC")
        return None

    return cmd, data


def send(cmd, data):
    global serial
    with lock:
        if (serial == None):
            print("Serial is not initialized")
            return
        if cmd not in commands:
            print("Invalid command: " + str(cmd))
            return
        try:
            serial.write(bytearray([SYNC_BYTE, cmd]))
            if (data != None):
                serial.write(data)
        except Exception as error:
            print("Exception while sending write command: " + str(error))

def serial_read(cmd, data = None):
    # Sends a read command with optional data
    send(cmd | READ_BYTE, data)

def serial_write(cmd, data):
    # Sends a write command with data
    send(cmd | WRITE_BYTE, data)