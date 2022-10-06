#!/usr/bin/env python3

import serial
import time
import ctypes
from ctypes import Structure
from struct import *
from JSON import *
from MonitorDefinition import *
from StateMachineDefinition import *
from Helpers import *
import threading
from CommuncationDefinition import *

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
        self.port = "/dev/ttyS0"
        self.baud = 256000
        self.serial = None
        self.lock = threading.Lock()

    @staticmethod
    def convert_to_bytes(st):
        buffer = ctypes.create_string_buffer(ctypes.sizeof(st))
        ctypes.memmove(buffer, ctypes.addressof(st), ctypes.sizeof(st))
        return buffer.raw

    @staticmethod
    def crc8(data):
        if (data == None):
            raise ValueError('Data is not valid: '+str(data))
        crc = 0
        for i in range(len(data)):
            byte = data[i]
            for b in range(8):
                fb_bit = (crc ^ byte) & 0x01
                if fb_bit == 0x01:
                    crc = crc ^ 0x18
                crc = (crc >> 1) & 0x7f
                if fb_bit == 0x01:
                    crc = crc | 0x80
                byte = byte >> 1
        return crc

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
        if (self.serial == None):
            raise ValueError('serial communication object is not initialized')
        if (n <= 0):
            raise ValueError('Invalid number of bytes to read: '+str(n))
        self.lock.acquire()
        buf = self.serial.read(n)
        if (len(buf) != n):
            print('Bytes dont match:'+str(buf))
            self.lock.release()
            return None
        crcBuf = self.serial.read(1)
        self.lock.release()
        if (len(crcBuf) == 0):
            print("No CRC sent")
            return None
        crc = unpack('B', crcBuf)[0]
        crccalc = self.crc8(buf)
        if (crc != crccalc):
            print("Invalid CRC:"+str(crc)+"Should be:"+str(crccalc) + " Recieved message: "+str(buf))
            return None
        return buf

    def __write(self, data, n=-1):
        if (n == -1):
            n = ctypes.sizeof(data)
        if (self.serial == None):
            raise ValueError('serial communication object is not initialized')
        self.lock.acquire()
        sent = self.serial.write(data)
        self.lock.release()
        if (sent != n):
            print("Bytes sent to not match specified bytes. Sent:" +
                  str(sent)+",Spec:"+str(n))

    def __writeCmd(self, cmd):
        if (self.serial == None):
            raise ValueError('serial communication object is not initialized')
        CMD = bytearray([cmd[0] | CMD_WRITE[0]])
        self.lock.acquire()
        self.serial.write(CMD_SYNC)
        self.serial.write(CMD)
        self.lock.release()

    def __readCmd(self, cmd):
        if (self.serial == None):
            raise ValueError('serial communication object is not initialized')
        self.lock.acquire()
        self.serial.write(CMD_SYNC)
        self.serial.write(cmd)
        self.lock.release()

    def start(self, port, baud):
        if self.serial is not None and self.serial.isOpen() == True:
            print("Disconnecting previous serial")
            self.serial.close()
        self.port = port
        self.baud = baud
        try:
            print("trying to get lock")
            self.lock.acquire()
            print("starig serial")
            self.serial = serial.Serial(self.port, self.baud, timeout=1, write_timeout=0)
            print("done")
            self.started = True
            self.serial.reset_input_buffer()
            self.lock.release()
        except serial.SerialException as error:
            self.lock.release()
            print(error)
            self.started = False
        return self.started

    def initialize(self):
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
        self.__readCmd(CMD_PING)
        buf = self.__read(1)
        if buf is not None:
            return int.from_bytes(buf, "big", signed=False)
        else:
            return -1

    def readStructure(self, structType):
        print("Reading structure of size: "+str(ctypes.sizeof(structType)))
        buf = self.__read(ctypes.sizeof(structType))
        if (buf == None):
            raise ValueError('invalid serial data')
        structure = structType.from_buffer_copy(buf)
        return structure

    def getMonitorData(self):
        self.__readCmd(CMD_DATA)
        try:
            return self.readStructure(MonitorData)
        except ValueError as error:
            print("Failed to get monitor data: "+str(error))

    def getMachineState(self):
        self.__readCmd(CMD_STATE)
        try:
            return self.readStructure(MachineState)
        except ValueError as error:
            print("Failed to get machine state: "+str(error))
            return None

    def getMachineProfile(self):
        self.__readCmd(CMD_MPROFILE)
        try:
            return self.readStructure(MachineProfile)
        except ValueError as error:
            print("Failed to get data: " + str(error))

    def getMachineConfiguration(self):
        self.__readCmd(CMD_MCONFIG)
        try:
            return self.readStructure(MachineConfiguration)
        except ValueError as error:
            print("Failed to get data: "+str(error))

    def getMachinePerformance(self):
        self.__readCmd(CMD_MPERFORMANCE)
        try:
            return self.readStructure(MachinePerformance)
        except ValueError as error:
            print("Failed to get data: "+str(error))

    def getMachinePerformance(self):
        self.__readCmd(CMD_MPERFORMANCE)
        try:
            return self.readStructure(MachinePerformance)
        except ValueError as error:
            print("Failed to get data: "+str(error))
            return None

    def getMotionProfile(self):
        self.__readCmd(CMD_MOTIONPROFILE)
        try:
            return self.readStructure(MotionProfile)
        except ValueError as error:
            print("Failed to get motion profile: "+str(error))
            return None

    def setFlashAddr(self, addr):
        self.__writeCmd(CMD_FLASHDATA)
        self.__write(self.convert_to_bytes(ctypes.c_int32(addr)),
                     ctypes.sizeof(ctypes.c_int32()))

    def getTestData(self):
        addr = 0
        dataList = []
        while True:
            try:
                self.setFlashAddr(addr)
                self.__readCmd(CMD_FLASHDATA)
                data = self.readStructure(MonitorData)
                print("Time:"+str(data.timems))
                if (data.timems < 0):
                    break
                dataList.append(data)
                addr += ctypes.sizeof(MonitorData)
            except ValueError as error:
                print("Failed to get data: "+str(error)+", trying again...")
                continue
        return dataList

    def getMotionStatus(self):
        self.__readCmd(CMD_MOTIONSTATUS)
        try:
            return self.readStructure(ctypes.c_int)
        except ValueError as error:
            print("Failed to get motion status: "+str(error))

    def getMotionMode(self):
        self.__readCmd(CMD_MOTIONMODE)
        try:
            return self.readStructure(ctypes.c_int)
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
