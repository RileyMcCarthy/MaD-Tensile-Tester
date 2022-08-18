#!/usr/bin/env python3
#https://docs.python.org/3/library/struct.html
import serial
import time
from struct import *
from ctypes import *

class MonitorData(Structure):
    _fields_ = [("forceRaw", c_uint32),
        ("encoderRaw", c_uint32),
        ("force", c_float),
        ("force", c_float),
        ("encoderRaw", c_uint32),
        ("encoderRaw", c_uint32)]

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyS0', 115200)
    ser.reset_input_buffer()
    while True:
        print("Waiting to recieve data:"+str(sizeof(MonitorData)));
        data = MonitorData.from_buffer_copy(ser.read(sizeof(MonitorData)))
        print(data.force)
    while True:
        packet = bytearray()
        packet.append(0x41)
        ser.write(packet)
        line = ser.readline()
        print(line)
        time.sleep(2.5)