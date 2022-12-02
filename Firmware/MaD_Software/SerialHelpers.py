#!/usr/bin/env python3

import serial
import ctypes
from struct import *
from definitions.JSON import *
from definitions.MonitorDefinition import *
from definitions.StateMachineDefinition import *
from definitions.CommuncationDefinition import *
from Helpers import *
import threading
import time
import multiprocessing

CMD_SYNC = 0x55
CMD_WRITE = 128
CMD_READ = 0
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
    CMD_PING: c_uint8,
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


class MaD_Serial:

    def __init__(self):
        self.started = False
        self.port = "/dev/serial0"
        self.baud = 1000000
        self.serial = None
        self.manager = multiprocessing.Manager()
        self.rx = self.manager.dict()
        self.quit = multiprocessing.Event()
        self.p = multiprocessing.Process(target=self.manage_serial, args=())
        # Rlock is used as it can be acuired by the same thread multiple times
        self.lock = threading.RLock()
        # Default settings
        self.start()
        self.initialize()

    def __del__(self):
        self.quit.set()
        if self.p.is_alive():
            self.p.join()

    @ staticmethod
    def convert_to_bytes(st):
        buffer = ctypes.create_string_buffer(ctypes.sizeof(st))
        ctypes.memmove(buffer, ctypes.addressof(st), ctypes.sizeof(st))
        return buffer.raw

    @ staticmethod
    def structEq(comp, other):
        return MaD_Serial.convert_to_bytes(comp) == MaD_Serial.convert_to_bytes(other)

    def manage_serial(self):
        # Thread to manage the serial port
        while not self.quit.is_set():
            if self.serial is None:
                time.sleep(0.1)
                print("Serial not started1")
                continue
            print("Parsing packet")
            self.parse_packet()
            monitorData = self.rx.pop(CMD_DATA, None)
            if monitorData is not None:
                pass
                # print(str(monitorData.timeus)+" "+str(monitorData.log))

    def parse_packet(self):
        # parses a packet from the serial buffer
        if (self.serial == None):
            print("Serial is not initialized")
            return
        with self.lock:
            # wait for the start byte
            try:
                self.serial.timeout = None  # wait forever
                start = None
                while (start != CMD_SYNC):
                    start = self.serial.read(1)[0]
                    if start is None:
                        print("No start byte found")
                        return
            except Exception as err:
                print("Exception waiting for start byte: " + str(err))
                return
            self.serial.timeout = 1  # 1 second timeout
            # read the command byte
            cmd = None
            try:
                cmd = self.serial.read(1)[0] & 0x7F
                if cmd is None:
                    print("No command byte found")
                    return
            except Exception as err:
                print("Exception reading command byte: " + str(err))
                return

            # check if the command is valid
            if cmd not in CMD_TYPE:
                print("Invalid command byte: " + str(cmd))
                return

            # read the data length
            length = None
            try:
                length = self.serial.read(1)[0]
                if length is None:
                    print("No length byte found")
                    return
            except Exception as err:
                print("Exception reading length byte: " + str(err))
                return

            # read the data
            data = None
            try:
                data = self.serial.read(length)
                if data is None:
                    print("No data found")
                    return
            except Exception as err:
                print("Exception reading data: " + str(err))
                return

            # read the crc
            crc = None
            try:
                crc = self.serial.read(1)[0]
                if crc is None:
                    print("No crc found")
                    return
            except Exception as err:
                print("Exception reading crc: " + str(err))
                return

            # check the crc
            # unpack('B', crcBuf)[0]
            if crc8(data) != crc:
                print("Invalid CRC")
                return

            # Parse the data into the correct structure and add it to the rx dictionary
            packet = CMD_TYPE[cmd].from_buffer_copy(data)
            self.rx[cmd] = packet

    def __write(self, data, size):
        # Writes data to the serial port
        with self.lock:
            if (self.serial == None):
                print("Serial is not initialized")
                return
            sent = self.serial.write(data)
            if (sent != size):
                print("Bytes sent to not match specified bytes. Sent:" +
                      str(sent)+",Spec:"+str(size))

    def __send_write_cmd(self, cmd):
        if (self.serial == None):
            print("Serial is not initialized")
            return
        self.__write(bytearray([CMD_SYNC, cmd | CMD_WRITE]), 2)

    def __send_read_cmd(self, cmd):
        if (self.serial == None):
            print("Serial is not initialized")
            return
        self.__write(bytearray([CMD_SYNC, cmd]), 2)

    def __read_data(self, cmd):
        end_time = time.time() + 1  # 1 second timeout
        while cmd not in self.rx:
            if time.time() > end_time:
                print("Timeout waiting for data: " + str(cmd))
                return None
        return self.rx.pop(cmd, None)

    def start(self):
        with self.lock:
            self.started = False
            if self.serial is not None and self.serial.isOpen():
                self.serial.close()
            try:
                print("Starting Serial on port: " + self.port)
                self.serial = serial.Serial(
                    self.port, self.baud, timeout=1, write_timeout=1)
                self.started = True
                self.serial.reset_input_buffer()
            except Exception as error:
                print("Unable to open serial: "+str(error))
                self.started = False
            self.p.start()
        return self.started

    def initialize(self):
        # wait until device is communicating
        print("Pinging device")
        version = self.getVersion()
        if version is None:
            print("Device not responding")
            #self.started = False
            return False
        if (version.value != MAD_VERSION):
            print("Incorrect Verion: "+str(version) +
                  " Expected: "+str(MAD_VERSION))
            #self.started = False
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
            #self.started = False
            return False
        print("Machine profile successfully updated")
        return True

    def getVersion(self):
        # Returns version of the device
        self.__send_read_cmd(CMD_PING)
        return self.__read_data(CMD_PING)

    def getMonitorData(self):
        self.__send_read_cmd(CMD_DATA)
        return self.__read_data(CMD_DATA)

    def getMachineState(self):
        self.__send_read_cmd(CMD_STATE)
        return self.__read_data(CMD_STATE)

    def getMachineProfile(self):
        self.__send_read_cmd(CMD_MPROFILE)
        return self.__read_data(CMD_MPROFILE)

    def getMachineConfiguration(self):
        self.__send_read_cmd(CMD_MCONFIG)
        return self.__read_data(CMD_MCONFIG)

    def getMachinePerformance(self):
        self.__send_read_cmd(CMD_MPERFORMANCE)
        return self.__read_data(CMD_MPERFORMANCE)

    def getMotionProfile(self):
        self.__send_read_cmd(CMD_MOTIONPROFILE)
        return self.__read_data(CMD_MOTIONPROFILE)

    def getMotionStatus(self):
        self.__send_read_cmd(CMD_MOTIONSTATUS)
        return self.__read_data(CMD_MOTIONSTATUS)

    def getMotionMode(self):
        self.__send_read_cmd(CMD_MOTIONMODE)
        return self.__read_data(CMD_MOTIONMODE)

    def setMachineProfile(self, profile):
        self.__send_write_cmd(CMD_MPROFILE)
        self.__write(self.convert_to_bytes(profile),
                     ctypes.sizeof(MachineProfile))

    def setMachineConfiguration(self, configuration):
        self.__send_write_cmd(CMD_MCONFIG)
        self.__write(self.convert_to_bytes(configuration),
                     ctypes.sizeof(MachineConfiguration))

    def setMachinePerformance(self, performance):
        self.__send_write_cmd(CMD_MPERFORMANCE)
        self.__write(self.convert_to_bytes(performance),
                     ctypes.sizeof(MachinePerformance))

    def setMotionProfile(self, profile):
        self.__send_write_cmd(CMD_MOTIONPROFILE)
        self.__write(self.convert_to_bytes(profile),
                     ctypes.sizeof(MotionProfile))

    def setMotionMode(self, mode):
        print("mode: "+str(self.convert_to_bytes(ctypes.c_int(mode))) +
              " " + str(ctypes.sizeof(ctypes.c_int)))
        self.__send_write_cmd(CMD_MOTIONMODE)
        self.__write(self.convert_to_bytes(
            ctypes.c_int(mode)), ctypes.sizeof(ctypes.c_int))

    def setMotionFunction(self, function, data):
        self.__send_write_cmd(CMD_MOTIONFUNCTION)
        print("function: "+str(ctypes.c_int(function)))
        self.__write(self.convert_to_bytes(
            ctypes.c_int(function)), ctypes.sizeof(ctypes.c_int))
        self.__write(self.convert_to_bytes(
            ctypes.c_int(data)), ctypes.sizeof(ctypes.c_int))

    def setMotionStatus(self, status):
        print("Status: "+str(self.convert_to_bytes(ctypes.c_int(status))) +
              " " + str(ctypes.sizeof(ctypes.c_int)))
        self.__send_write_cmd(CMD_MOTIONSTATUS)
        self.__write(self.convert_to_bytes(
            ctypes.c_int(status)), ctypes.sizeof(ctypes.c_int))
