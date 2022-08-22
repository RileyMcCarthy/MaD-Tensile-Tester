#!/usr/bin/env python3

import serial
import time
import serial
import ctypes
from ctypes import Structure
from struct import *
from JSON import *

CMD_WRITE = bytearray([128])
CMD_READ = bytearray([0])
CMD_PING = bytearray([0])
CMD_DATA = bytearray([1])
CMD_STATE = bytearray([2])
CMD_MPROFILE = bytearray([3])
CMD_MCONFIG = bytearray([4])
CMD_MPERFORMANCE = bytearray([5])

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
    CMD = bytearray([CMD_MPROFILE[0] | CMD_WRITE[0]])
    serial.write(CMD)


def writeSerial(serial, data, n):
    sent = serial.write(data)
    if (sent != n):
        print("Bytes sent to not match specified bytes. Sent:"+str(sent)+",Spec:"+str(n))

# Returns true if ping is successful
def getPing(serial):
    serial.write(CMD_PING)
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
    serial.write(CMD_DATA)
    try:
        return readStructure(serial,MonitorData_s)
    except ValueError as error:
        raise

def getMachineProfile(serial):
    serial.write(CMD_MPROFILE)
    try:
        return readStructure(serial,MachineProfile_s)
    except ValueError as error:
        raise

def getMachineConfiguration(serial):
    serial.write(CMD_MCONFIG)
    try:
        return readStructure(serial,MachineConfiguration_s)
    except ValueError as error:
        raise

def getMachinePerformance(serial):
    serial.write(CMD_MPERFORMANCE)
    try:
        return readStructure(serial,MachinePerformance)
    except ValueError as error:
        raise

def convert_struct_to_bytes(st):
    buffer = ctypes.create_string_buffer(ctypes.sizeof(st))
    ctypes.memmove(buffer, ctypes.addressof(st), ctypes.sizeof(st))
    return buffer.raw

def setMachineProfile(serial, profile):
    writeCmd(serial,CMD_MPROFILE)
    writeSerial(serial, convert_struct_to_bytes(profile), ctypes.sizeof(profile))