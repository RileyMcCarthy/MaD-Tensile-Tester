#!/usr/bin/env python3

import serial
import time
import serial
import ctypes
from ctypes import Structure
from struct import *
from JSON import *
from MonitorDefinition import *
from StateMachineDefinition import *

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
CMD_FLASHDATA = bytearray([10])

def crc8(data):
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

def getBoolean(packet, successful = None):
    if (packet):
        return unpack('?', packet)[0]
    else:
        raise ValueError('packet parameter is empty')
## Reads serial for n number of bytes, returns true if bytes read == n
#def readSerial(serial, n):
#    attempts = 0
#    while True:
#        attempts+=1
#        if (attempts > 5):
#            print("Unable to read data, see previous logs for errors")
#            return None
#        buf = serial.read(n)
#        if (len(buf) != n):
#            print('Bytes dont match:'+str(buf))
#            serial.write(0)
#            return None
#        crcBuf = serial.read(1)
#        if (len(crcBuf)==0):
#            print("No CRC sent")
#            serial.write(0)
#            return None
#        crc = unpack('B',crcBuf)[0]
#        crccalc = crc8(buf)
#        if (crc != crccalc):
#            print("Invalid CRC:"+str(crc)+"Should be:"+str(crccalc))
#            continue
#        serial.write(crccalc)
#        return buf
# Reads serial for n number of bytes, returns true if bytes read == n
def readSerial(serial, n):
    buf = serial.read(n)
    if (len(buf) != n):
        print('Bytes dont match:'+str(buf))
        return None
    crcBuf = serial.read(1)
    if (len(crcBuf)==0):
        print("No CRC sent")
        return None
    crc = unpack('B',crcBuf)[0]
    crccalc = crc8(buf)
    if (crc != crccalc):
        print("Invalid CRC:"+str(crc)+"Should be:"+str(crccalc))
        return None
    return buf

def writeCmd(serial, cmd):
    CMD = bytearray([cmd[0] | CMD_WRITE[0]])
    serial.write(CMD_SYNC)
    serial.write(CMD)

def readCmd(serial, cmd):
    serial.write(CMD_SYNC)
    serial.write(cmd)

def writeSerial(serial, data, n):
    if (serial == None):
        raise ValueError('serial parameter is empty')
    sent = serial.write(data)
    if (sent != n):
        print("Bytes sent to not match specified bytes. Sent:"+str(sent)+",Spec:"+str(n))

# Returns true if ping is successful
def getPing(serial):
    readCmd(serial,CMD_PING)
    buf = readSerial(serial,1)
    try:
        return getBoolean(buf)
    except ValueError as error:
        return False

def readStructure(serial, structType):
    buf = readSerial(serial, ctypes.sizeof(structType))
    if (buf == None):
        raise ValueError('invalid serial data')
    structure = structType.from_buffer_copy(buf)
    return structure

def getMonitorData(serial):
    readCmd(serial,CMD_DATA)
    try:
        return readStructure(serial,MonitorData)
    except ValueError as error:
        print("Failed to get monitor data: "+str(error))

def getMachineState(serial):
    readCmd(serial,CMD_STATE)
    try:
        return readStructure(serial,MachineState)
    except ValueError as error:
        print("Failed to get machine state: "+str(error))
        serial.reset_input_buffer()

def getMachineProfile(serial):
    readCmd(serial,CMD_MPROFILE)
    try:
        return readStructure(serial,MachineProfile)
    except ValueError as error:
        print("Failed to get data: "+ str(error))

def getMachineConfiguration(serial):
    readCmd(serial,CMD_MCONFIG)
    try:
        return readStructure(serial,MachineConfiguration)
    except ValueError as error:
        print("Failed to get data: "+str(error))

def getMachinePerformance(serial):
    readCmd(serial,CMD_MPERFORMANCE)
    try:
        return readStructure(serial,MachinePerformance)
    except ValueError as error:
        print("Failed to get data: "+str(error))

def getMachinePerformance(serial):
    readCmd(serial,CMD_MPERFORMANCE)
    try:
        return readStructure(serial,MachinePerformance)
    except ValueError as error:
        print("Failed to get data: "+str(error))
        return None

def getMotionProfile(serial):
    readCmd(serial,CMD_MOTIONPROFILE)
    try:
        return readStructure(serial,MotionProfile)
    except ValueError as error:
        print("Failed to get motion profile: "+str(error))
        return None

def getTestData(serial):
    readCmd(serial,CMD_FLASHDATA)
    try:
        while True:
            data = readStructure(serial,MonitorData)
            print("Time:"+str(data.timems))
            if (data.timems < 0):
                break
    except ValueError as error:
        print("Failed to get data: "+str(error))
        return None

def convert_to_bytes(st):
    buffer = ctypes.create_string_buffer(ctypes.sizeof(st))
    ctypes.memmove(buffer, ctypes.addressof(st), ctypes.sizeof(st))
    return buffer.raw

def structEq(comp, other):
    return convert_to_bytes(comp) == convert_to_bytes(other)

def setMachineProfile(serial, profile):
    writeCmd(serial,CMD_MPROFILE)
    writeSerial(serial, convert_to_bytes(profile), ctypes.sizeof(MachineProfile))

def setMachineConfiguration(serial, configuration):
    writeCmd(serial,CMD_MCONFIG)
    writeSerial(serial,convert_to_bytes(configuration),ctypes.sizeof(MachineConfiguration))

def setMachinePerformance(serial, performance):
    writeCmd(serial,CMD_MPERFORMANCE)
    writeSerial(serial, convert_to_bytes(performance), ctypes.sizeof(MachinePerformance))

def setMotionProfile(serial, profile):
    writeCmd(serial,CMD_MOTIONPROFILE)
    writeSerial(serial, convert_to_bytes(profile), ctypes.sizeof(MotionProfile))

def setMotionMode(serial, mode):
    print("mode: "+str(convert_to_bytes(ctypes.c_int(mode))) + " " + str(ctypes.sizeof(ctypes.c_int)))
    writeCmd(serial,CMD_MOTIONMODE)
    writeSerial(serial, convert_to_bytes(ctypes.c_int(mode)), ctypes.sizeof(ctypes.c_int))

def setMotionFunction(serial, function, data):
    writeCmd(serial,CMD_MOTIONFUNCTION)
    print("function: "+str(ctypes.c_int(function)))
    writeSerial(serial, convert_to_bytes(ctypes.c_int(function)), ctypes.sizeof(ctypes.c_int))
    writeSerial(serial, convert_to_bytes(ctypes.c_int(data)), ctypes.sizeof(ctypes.c_int))

