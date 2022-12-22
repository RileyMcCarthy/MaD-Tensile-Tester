import time
import ctypes
from definitions.JSON import *
from definitions.MonitorDefinition import *
from definitions.StateMachineDefinition import *
from definitions.CommunicationDefinition import *
from multiprocessing import Manager, Lock, Process
from SerialHelpers import MaD_Serial
from app import socketio
from serial import Serial
from Helpers import crc8

mSerial = None

CMD_PING = 0
CMD_DATA = 1
CMD_STATE = 2
CMD_MPROFILE = 3
CMD_MCONFIG = 4
CMD_MPERFORMANCE = 5
CMD_MOTIONPROFILE = 6
CMD_MOTIONMODE = 7
CMD_MOTIONFUNCTION = 8
CMD_MOTIONSTATUS = 9
CMD_FLASHDATA = 10

CMD_TYPE = {
    CMD_PING: ctypes.c_uint8,
    CMD_DATA: MonitorDataPacket,
    CMD_STATE: MachineState,
    CMD_MPROFILE: MachineProfile,
    CMD_MCONFIG: MachineConfiguration,
    CMD_MPERFORMANCE: MachinePerformance,
    CMD_MOTIONPROFILE: MotionProfile,
    CMD_MOTIONMODE: MotionMode,
    CMD_MOTIONFUNCTION: c_int32,
    CMD_MOTIONSTATUS: MotionStatus,
}


def serial_thread(rx, tx, lock, quit):
    # Thread to manage the serial port
    # rx is dict of returned cmd types
    # tx is dict of cmd types to send
    # quit is event to stop thread
    # tx will by emptied by this thread FiFO
    # rx will be filled by this thread
    def send(ser, send_queue):
        if (ser == None):
            print("Serial is not initialized")
            return
        while not send_queue.empty():
            try:
                cmd = send_queue.get()
                ser.write(cmd)
                print("Sending: " + str(cmd))
            except Exception as error:
                print("Exception while sending: " + str(error))
                return

    def recieve(ser, recieve_dict):
        # parses a packet from the serial buffer
        if (ser == None):
            print("Serial is not initialized")
            return False

        # check if there is data in the buffer
        if (ser.in_waiting < 1):
            return False

        # wait for the start byte
        try:
            start = None
            while start != MaD_Serial.SYNC_BYTE:
                start = ser.read(1)[0]
                if start is None:
                    print("No start byte found")
                    return False
        except Exception as err:
            print("Exception waiting for start byte: " + str(err))
            return False

        # read the command byte
        cmd = None
        try:
            cmd = ser.read(1)[0] & 0x7F
            if cmd is None:
                print("No command byte found")
                return False
        except Exception as err:
            print("Exception reading command byte: " + str(err))
            return False

        # check if the command is valid
        if cmd not in CMD_TYPE:
            print("Invalid command byte: " + str(cmd))
            return False

        # read the data length
        length = None
        try:
            length = ser.read(1)[0]
            if length is None:
                print("No length byte found")
                return False
        except Exception as err:
            print("Exception reading length byte: " + str(err))
            return False

        # read the data
        data = None
        try:
            data = ser.read(length)
            if data is None:
                print("No data found")
                return False
        except Exception as err:
            print("Exception reading data: " + str(err))
            return False

        # read the crc
        crc = None
        try:
            crc = ser.read(1)[0]
            if crc is None:
                print("No crc found")
                return False
        except Exception as err:
            print("Exception reading crc: " + str(err))
            return False

        # check the crc
        if crc8(data) != crc:
            print("Invalid CRC")
            return False

        # Parse the data into the correct structure and add it to the rx dictionary
        packet = CMD_TYPE[cmd].from_buffer_copy(data)
        recieve_dict[int(cmd)] = packet
        #if cmd == CMD_DATA:
        print("Recieved(" + str(cmd)+"): " + str(recieve_dict[int(cmd)]))
        return True

    mad_serial = None
    port = "/dev/serial0"
    baud = 1000000
    while not quit.is_set():
        try:
            if mad_serial is not None and mad_serial.isOpen():
                mad_serial.close()
            print("Starting Serial on port: " + port)
            mad_serial = Serial(port, baud, timeout=1, write_timeout=1)
            mad_serial.reset_input_buffer()
            break
        except Exception as error:
            print("Unable to open serial: "+str(error))
        time.sleep(1)
        print("Retrying serial connection...")

    print("Serial connected using: "+mad_serial.name)
    while not quit.is_set():
        try:
            send(mad_serial, tx)
            recieve(mad_serial, rx)
            time.sleep(0.01)
        except Exception as error:
            print("Exception in serial thread: " + str(error))


def start_serial_thread(event):
    manager = Manager()
    serial_rx = manager.dict()
    serial_tx = manager.Queue()
    serial_lock = manager.Lock()
    global mSerial
    mSerial = MaD_Serial(serial_rx, serial_tx, serial_lock)
    p = Process(target=serial_thread, args=(
        serial_rx, serial_tx, serial_lock, event))
    return p


def initialize(self):
    # wait until device is communicating
    print("Pinging device")
    self.getVersion()
    version = None
    if version is None:
        print("Device not responding")
        return False

    if (version.value != MAD_VERSION):
        print("Incorrect Verion: "+str(version) +
              " Expected: "+str(MAD_VERSION))
        return False
    print("Device responded")

    # Send machine profile
    print("Sending machine profile")
    profile = loadMachineProfile()
    self.setMachineProfile(profile)

    # Check machine machine has updated
    print("Confirmin machine profille is valid")
    deviceProfile = self.getMachineProfile()
    if (deviceProfile == None or self.structEq(profile, deviceProfile) == False):
        print("Profile did not update, trying again")
        return False
    print("Machine profile successfully updated")
    return True


def get_command(cmd, timeout):
    # wait for a command to be recieved
    # timeout is in seconds
    def wait_for_res():
        start = time.time()
        data = None
        global mSerial
        while data is None:
            if time.time() - start > timeout:
                print("Timeout waiting for command: " + str(cmd))
                return None
        data = mSerial.read_data(cmd)
        socketio.sleep(0.01)
        print(data)
    global mSerial
    mSerial.send_read_cmd(cmd)
    socketio.start_background_task(
        wait_for_res)


@socketio.on('machine_config', namespace='/serial')
def test():
    print("Testing the serial endpoint")
    get_command(CMD_MCONFIG, 3)


def getVersion():
    # Returns version of the device
    global mSerial
    mSerial.__send_read_cmd(CMD_PING)
    return mSerial.__read_data(CMD_PING)


def hasVersion(self):
    return self.__read_data(CMD_PING)


def getMonitorData(self):
    self.__send_read_cmd(CMD_DATA)
    # return self.__read_data(CMD_DATA)


def getMachineState(self):
    self.__send_read_cmd(CMD_STATE)
    # return self.__read_data(CMD_STATE)


def getMachineProfile(self):
    self.__send_read_cmd(CMD_MPROFILE)
    # return self.__read_data(CMD_MPROFILE)


def getMachineConfiguration(self):
    self.__send_read_cmd(CMD_MCONFIG)
    # return self.__read_data(CMD_MCONFIG)


def getMachinePerformance(self):
    self.__send_read_cmd(CMD_MPERFORMANCE)
    # return self.__read_data(CMD_MPERFORMANCE)


def getMotionProfile(self):
    self.__send_read_cmd(CMD_MOTIONPROFILE)
    # return self.__read_data(CMD_MOTIONPROFILE)


def getMotionStatus(self):
    self.__send_read_cmd(CMD_MOTIONSTATUS)
    # return self.__read_data(CMD_MOTIONSTATUS)


def getMotionMode(self):
    self.__send_read_cmd(CMD_MOTIONMODE)
    # return self.__read_data(CMD_MOTIONMODE)


def setMachineProfile(self, profile):
    self.__send_write_cmd(CMD_MPROFILE, self.convert_to_bytes(profile))


def setMachineConfiguration(self, configuration):
    self.__send_write_cmd(
        CMD_MCONFIG, self.convert_to_bytes(configuration))


def setMachinePerformance(self, performance):
    self.__send_write_cmd(
        CMD_MPERFORMANCE, self.convert_to_bytes(performance))


def setMotionProfile(self, profile):
    self.__send_write_cmd(
        CMD_MOTIONPROFILE, self.convert_to_bytes(profile))


def setMotionMode(self, mode):
    self.__send_write_cmd(
        CMD_MOTIONMODE, self.convert_to_bytes(ctypes.c_int(mode)))


def setMotionStatus(self, status):
    self.__send_write_cmd(
        CMD_MOTIONSTATUS, self.convert_to_bytes(ctypes.c_int(status)))
