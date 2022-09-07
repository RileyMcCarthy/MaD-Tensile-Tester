#!/usr/bin/env python3
# https://docs.python.org/3/library/struct.html

# use Kivy for GUI
from SerialHelpers import *
from GUI import *
import numpy as np
import matplotlib.pyplot as plt


def print_ctypes_obj(obj, level=0):

    delta_indent = "    "
    indent = delta_indent*level

    # Assess wether the object is an array, a structure or an elementary type
    if issubclass(type(obj), ctypes.Array):
        print('{}ARRAY {}'.format(indent, obj))
        for obj2 in obj:
            print_ctypes_obj(obj2, level+1)

    elif hasattr(obj, '_fields_'):
        print('{}STRUCTURE {}'.format(indent, obj))
        for fdesc in obj._fields_:
            # Get the next field descriptor
            fname = fdesc[0]
            ftype = fdesc[1]
            if len(fdesc) == 3:
                fbitlen = fdesc[2]
            else:
                fbitlen = 8*ctypes.sizeof(ftype)
            obj2 = getattr(obj, fname)
            print('{}FIELD {} (type={}, bitlen={})'.format(
                indent+delta_indent, fname, ftype, fbitlen))
            print_ctypes_obj(obj2, level+2)

    else:
        print('{}VALUE = {} (type={})'.format(indent, obj, type(obj)))


def loadMotionProfile():
    motionProfile = MotionProfile()
    motionProfile.name = b'Test1.mp'
    motionProfile.setCount = 1
    motionProfile.sets[0].number = 1
    motionProfile.sets[0].executions = 2
    motionProfile.sets[0].quartetCount = 1

    motionProfile.sets[0].quartets[0].name = b'qrt1.qrt'
    motionProfile.sets[0].quartets[0].function = 1
    motionProfile.sets[0].quartets[0].parameters[0] = 50
    motionProfile.sets[0].quartets[0].parameters[1] = 10
    motionProfile.sets[0].quartets[0].dwell = 500
    return motionProfile


def loadMachineProfile():
    machineProfile = MachineProfile()
    machineProfile.name = b"Tensile_Test_1"

    machineProfile.number = 1

    # MachineConfiguration
    machineProfile.configuration.motorType = b"640-DST"

    machineProfile.configuration.maxMotorTorque = 3.82
    machineProfile.configuration.maxMotorRPM = 5000
    machineProfile.configuration.gearDiameter = 40.0
    machineProfile.configuration.gearPitch = 2
    machineProfile.configuration.systemIntertia = 0.00121
    machineProfile.configuration.staticTorque = 0.558
    machineProfile.configuration.load = 37.8

    machineProfile.configuration.positionEncoderType = b"QuadEncoder"

    machineProfile.configuration.positionEncoderStepsPerRev = 4 * 2048

    machineProfile.configuration.forceGauge = b"DS2-5N"

    machineProfile.configuration.forceGaugeScaleFactor = -657.959
    machineProfile.configuration.forceGaugeZeroFactor = 16119601

    # MachinePerformance
    machineProfile.performance.minPosition = 0.01
    machineProfile.performance.maxPosition = 50.5
    machineProfile.performance.maxVelocity = 200.5
    machineProfile.performance.maxAcceleration = 100.5
    machineProfile.performance.maxForceTensile = 5.0
    machineProfile.performance.maxForceCompression = 5.0
    machineProfile.performance.forceGaugeNeutralOffset = 0.9

    return machineProfile


def initializeDevice(ser):
    while True:
        # wait until device is communicating
        print("Pinging device")
        if (getPing(ser) == False):
            print("Device failed to respond")
            time.sleep(2)
            continue
        print("Device responded")
        # Send machine profile
        print("Sending machine profile")
        profile = loadMachineProfile()
        setMachineProfile(ser, profile)
        print("Confirmin machine profille is valid")
        # Check machine machine has updated
        deviceProfile = getMachineProfile(ser)
        if (deviceProfile == None or structEq(profile, deviceProfile) == False):
            print("Profile did not update, trying again")
            time.sleep(2)
            continue
        print("Machine profile successfully updated")
        print_ctypes_obj(deviceProfile)

        break


if __name__ == '__main__':
    MaD().run()
    ser = serial.Serial('/dev/ttyS0', 115200, timeout=3)
    ser.reset_input_buffer()
    initializeDevice(ser)
    time.sleep(1)
    # while True:
    #    state = getMachineState(ser)
    #    print_ctypes_obj(state)
    #    time.sleep(1)
    dataList = getTestData(ser)
    x = []
    y = []
    for data in dataList:
        x = data.timeus
        y = data.force
    plt.scatter(x, y, alpha=0.5)
    plt.show()
    sys.exit()
    setMotionMode(ser, 1)
    machineState = getMachineState(ser)
    print_ctypes_obj(machineState)
    monitorData = getMonitorData(ser)
    print_ctypes_obj(monitorData)
    motionProfile = loadMotionProfile()
    setMotionProfile(ser, motionProfile)
    setMotionMode(ser, 2)
    sys.exit()

    motionProfile = loadMotionProfile()
    setMotionProfile(ser, motionProfile)
    protest = getMotionProfile(ser)
    protest = getMotionProfile(ser)
    print_ctypes_obj(protest)
    # while True:
    #print("Pinging device")
    # if (getPing(ser)):
    #    print("Device responded")
    # else:
    #    print("Device failed to respond")
    # try:
    #    data = getMonitorData(ser)
    #    print_ctypes_obj(data)
    # except ValueError as error:
    #    print(error)
    # try:
    #    print("Main Liip")
    #    data = getMachineProfile(ser)
    #    print_ctypes_obj(data)
    #    profile = MachineProfile()
    #    #setMachineProfile(ser, profile)
    # except ValueError as error:
    #    print(error)
    # time.sleep(3)
