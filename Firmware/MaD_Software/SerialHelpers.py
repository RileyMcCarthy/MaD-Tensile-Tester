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

CMD_SYNC = bytearray([0x55])
CMD_WRITE = bytearray([128])
CMD_READ = bytearray([0])
CMD_PING = bytearray([0])
CMD_DATA = bytearray([1])
CMD_STATE = bytearray([2])
CMD_MPROFILE = bytearray([3])
CMD_MCONFIG = bytearray([4])
CMD_MPERFORMANCE = bytearray([5])
CMD_MOTIONPROFILE = bytearray([6])
CMD_MOTIONMODE = bytearray([7])
CMD_MOTIONFUNCTION = bytearray([8])
CMD_MOTIONSTATUS = bytearray([9])
CMD_FLASHDATA = bytearray([10])


class MaD_Serial:

    def __init__(self):
        self.started = False
        self.port = "/dev/serial0"
        self.baud = 256000
        self.serial = None
        # Rlock is used as it can be acuired by the same thread multiple times
        self.lock = threading.RLock()
        # Default settings
        if self.start("/dev/serial0", 256000):
            self.initialize()

    @staticmethod
    def convert_to_bytes(st):
        buffer = ctypes.create_string_buffer(ctypes.sizeof(st))
        ctypes.memmove(buffer, ctypes.addressof(st), ctypes.sizeof(st))
        return buffer.raw

    @staticmethod
    def structEq(comp, other):
        return MaD_Serial.convert_to_bytes(comp) == MaD_Serial.convert_to_bytes(other)

    @staticmethod
    def getBoolean(packet):
        if (packet):
            return unpack('?', packet)[0]
        else:
            raise ValueError('packet parameter is empty')

    def __read(self, n):
        with self.lock:
            if (self.serial == None):
                return None
            if (n <= 0):
                return None
            try:
                buf = self.serial.read(n)
            except Exception as err:
                print("serial read exception: " + str(err))
                self.serial.reset_input_buffer()
                return None

            if (len(buf) != n):
                print('Bytes dont match:'+str(buf))
                self.serial.reset_input_buffer()
                return None
            try:
                crcBuf = self.serial.read(1)
            except Exception as err:
                print("serial read exception: " + str(err))
                self.serial.reset_input_buffer()
                return None

            if (len(crcBuf) == 0):
                print("No CRC sent")
                self.serial.reset_input_buffer()
                return None

            crc = unpack('B', crcBuf)[0]
            crccalc = crc8(buf)

            if (crc != crccalc):
                print("Invalid CRC:"+str(crc)+"Should be:" +
                      str(crccalc) + " Recieved message: "+str(buf))
                self.serial.reset_input_buffer()
                return None
            return buf

    def __write(self, data, n=-1):
        with self.lock:
            if (n == -1):
                n = ctypes.sizeof(data)
            if (self.serial == None):
                raise ValueError(
                    'serial communication object is not initialized')
            sent = self.serial.write(data)
            if (sent != n):
                print("Bytes sent to not match specified bytes. Sent:" +
                      str(sent)+",Spec:"+str(n))

    def __writeCmd(self, cmd):
        with self.lock:
            if (self.serial == None):
                raise ValueError(
                    'serial communication object is not initialized')
            CMD = bytearray([cmd[0] | CMD_WRITE[0]])
            self.serial.write(CMD_SYNC)
            self.serial.write(CMD)

    def __readCmd(self, cmd):
        with self.lock:
            if (self.serial == None):
                raise ValueError(
                    'serial communication object is not initialized')
            self.serial.write(CMD_SYNC)
            self.serial.write(cmd)

    def start(self, port, baud):
        with self.lock:
            if self.serial is not None and self.serial.isOpen() == True:
                print("Disconnecting previous serial")
                self.serial.close()
            self.port = port
            self.baud = baud
            try:
                print("starig serial")
                self.serial = serial.Serial(
                    self.port, self.baud, timeout=1, write_timeout=1)
                self.started = True
                self.serial.reset_input_buffer()
            except serial.SerialException as error:
                print(error)
                self.started = False
            return self.started

    def initialize(self):
        with self.lock:
            # wait until device is communicating
            print("Pinging device")
            version = self.getVersion()
            print(version)
            if (version != MAD_VERSION):
                print("Device failed to respond")
                self.started = False
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
                self.started = False
                return False
            print("Machine profile successfully updated")
            # print(structure_to_html(deviceProfile))
            self.started = True
            return True

    # Returns true if ping is successful

    def getVersion(self):
        with self.lock:
            self.__readCmd(CMD_PING)
            buf = self.__read(1)
            if buf is not None:
                return int.from_bytes(buf, "big", signed=False)
            else:
                return -1

    def readStructure(self, structType):
        with self.lock:
            buf = self.__read(ctypes.sizeof(structType))
            if (buf == None):
                raise ValueError('invalid serial data')
            structure = structType.from_buffer_copy(buf)
            return structure

    def getMonitorData(self):
        with self.lock:
            self.__readCmd(CMD_DATA)
            try:
                return self.readStructure(MonitorData)
            except ValueError as error:
                print("Failed to get monitor data: "+str(error))

    def getMachineState(self):
        with self.lock:
            try:
                self.__readCmd(CMD_STATE)
                return self.readStructure(MachineState)
            except ValueError as error:
                print("Failed to get machine state: "+str(error))
                return None

    def getMachineProfile(self):
        with self.lock:
            self.__readCmd(CMD_MPROFILE)
            try:
                return self.readStructure(MachineProfile)
            except ValueError as error:
                print("Failed to get data: " + str(error))

    def getMachineConfiguration(self):
        with self.lock:
            self.__readCmd(CMD_MCONFIG)
            try:
                return self.readStructure(MachineConfiguration)
            except ValueError as error:
                print("Failed to get data: "+str(error))

    def getMachinePerformance(self):
        with self.lock:
            self.__readCmd(CMD_MPERFORMANCE)
            try:
                return self.readStructure(MachinePerformance)
            except ValueError as error:
                print("Failed to get data: "+str(error))

    def getMachinePerformance(self):
        with self.lock:
            self.__readCmd(CMD_MPERFORMANCE)
            try:
                return self.readStructure(MachinePerformance)
            except ValueError as error:
                print("Failed to get data: "+str(error))
                return None

    def getMotionFunction(self):
        with self.lock:
            self.__readCmd(CMD_MOTIONFUNCTION)
            try:
                func = self.readStructure(c_int32).value
                data = self.readStructure(c_int32).value
                return func, data
            except ValueError as error:
                print("Failed to get data: "+str(error))
                return None

    def getMotionProfile(self):
        with self.lock:
            self.__readCmd(CMD_MOTIONPROFILE)
            try:
                return self.readStructure(MotionProfile)
            except ValueError as error:
                print("Failed to get motion profile: "+str(error))
                return None

    def setFlashAddr(self, addr):
        with self.lock:
            self.__writeCmd(CMD_FLASHDATA)
            self.__write(self.convert_to_bytes(ctypes.c_int32(addr)),
                         ctypes.sizeof(ctypes.c_int32()))

    def getTestData(self, addr=0):
        with self.lock:
            try:
                print("getting addr"+str(addr))
                self.setFlashAddr(addr)
                self.__readCmd(CMD_FLASHDATA)
                data = self.readStructure(MonitorData)
                print("Time:"+str(data.timems))
                addr += ctypes.sizeof(MonitorData)
                return data, addr
            except ValueError as error:
                print("Failed to get data: "+str(error))
                return None

    def getMotionStatus(self):
        try:
            self.__readCmd(CMD_MOTIONSTATUS)
            return self.readStructure(ctypes.c_int).value
        except ValueError as error:
            print("Failed to get motion status: "+str(error))

    def getMotionMode(self):
        try:
            self.__readCmd(CMD_MOTIONMODE)
            return self.readStructure(ctypes.c_int).value
        except ValueError as error:
            print("Failed to get motion mode: "+str(error))

    def setMachineProfile(self, profile):
        self.__writeCmd(CMD_MPROFILE)
        self.__write(self.convert_to_bytes(profile),
                     ctypes.sizeof(MachineProfile))

    def setMachineConfiguration(self, configuration):
        self.__writeCmd(CMD_MCONFIG)
        self.__write(self.convert_to_bytes(configuration),
                     ctypes.sizeof(MachineConfiguration))

    def setMachinePerformance(self, performance):
        self.__writeCmd(CMD_MPERFORMANCE)
        self.__write(self.convert_to_bytes(performance),
                     ctypes.sizeof(MachinePerformance))

    def setMotionProfile(self, profile):
        self.__writeCmd(CMD_MOTIONPROFILE)
        self.__write(self.convert_to_bytes(profile),
                     ctypes.sizeof(MotionProfile))

    def setMotionMode(self, mode):
        print("mode: "+str(self.convert_to_bytes(ctypes.c_int(mode))) +
              " " + str(ctypes.sizeof(ctypes.c_int)))
        self.__writeCmd(CMD_MOTIONMODE)
        self.__write(self.convert_to_bytes(
            ctypes.c_int(mode)), ctypes.sizeof(ctypes.c_int))

    def setMotionFunction(self, function, data):
        self.__writeCmd(CMD_MOTIONFUNCTION)
        print("function: "+str(ctypes.c_int(function)))
        self.__write(self.convert_to_bytes(
            ctypes.c_int(function)), ctypes.sizeof(ctypes.c_int))
        self.__write(self.convert_to_bytes(
            ctypes.c_int(data)), ctypes.sizeof(ctypes.c_int))

    def setMotionStatus(self, status):
        print("Status: "+str(self.convert_to_bytes(ctypes.c_int(status))) +
              " " + str(ctypes.sizeof(ctypes.c_int)))
        self.__writeCmd(CMD_MOTIONSTATUS)
        self.__write(self.convert_to_bytes(
            ctypes.c_int(status)), ctypes.sizeof(ctypes.c_int))
