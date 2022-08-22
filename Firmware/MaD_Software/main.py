#!/usr/bin/env python3
#https://docs.python.org/3/library/struct.html

#use Kivy for GUI
from SerialHelpers import *

def structEq(self, other):
    for fld in self._fields_:
        if getattr(self, fld[0]) != getattr(other, fld[0]):
            return False
    return True

def structNe(self, other):
    for fld in self._fields_:
        if getattr(self, fld[0]) != getattr(other, fld[0]):
            return True
    return False

def print_ctypes_obj(obj, level=0):

    delta_indent="    "
    indent=delta_indent*level

    # Assess wether the object is an array, a structure or an elementary type
    if issubclass(type(obj), ctypes.Array) :
        print('{}ARRAY {}'.format(indent, obj))
        for obj2 in obj:
            print_ctypes_obj(obj2, level+1)

    elif hasattr(obj, '_fields_'):
        print('{}STRUCTURE {}'.format(indent, obj))
        for fdesc in obj._fields_:
            # Get the next field descriptor
            fname = fdesc[0]
            ftype = fdesc[1]
            if len(fdesc)==3:
                fbitlen = fdesc[2]
            else:
                fbitlen = 8*ctypes.sizeof(ftype)
            obj2 = getattr(obj, fname)
            print('{}FIELD {} (type={}, bitlen={})'.format(indent+delta_indent, fname, ftype, fbitlen))
            print_ctypes_obj(obj2, level+2)

    else:
        print('{}VALUE = {} (type={})'.format(indent, obj, type(obj)))

def loadMotionProfile():
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

    machineProfile.configuration.forceGaugeScaleFactor = 1.0
    machineProfile.configuration.forceGaugeZeroFactor = 0

    # MachinePerformance
    machineProfile.performance.minPosition = 0.01
    machineProfile.performance.maxPosition = 50.5
    machineProfile.performance.maxVelocity = 200.5
    machineProfile.performance.maxAcceleration = 100.5
    machineProfile.performance.maxForceTensile = 3.5
    machineProfile.performance.maxForceCompression = 3.1
    machineProfile.performance.forceGaugeNeutralOffset = 0.9

    return machineProfile

def initializeDevice():
    profile = loadMotionProfile()

    while True:
        # wait until device is communicating
        print("Pinging device")
        if (getPing(ser) == False):
            print("Device failed to respond")
            time.sleep(2)
            continue
        print("Device responded")

        # Send machine profile
        print("Sending motion profile")
        profile = loadMotionProfile()
        setMachineProfile(ser, profile)

        print("Confirmin motion profille is valid")
        # Check machine profile has updated
        deviceProfile = getMachineProfile(ser)
        if (structEq(profile,deviceProfile)):
            print("Profile did not update, trying again")
            time.sleep(2)
            continue
        print("Motion profile successfully updated")
        break
#MotionMode,ModeFunctions/functionData inside MachineState
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyS0', 115200, timeout=1)
    ser.reset_input_buffer()

    initializeDevice()
    
    while True:
        #print("Pinging device")
        #if (getPing(ser)):
        #    print("Device responded")
        #else:
        #    print("Device failed to respond")
        #try:
        #    data = getMonitorData(ser)
        #    print_ctypes_obj(data)
        #except ValueError as error:
        #    print(error)
        try:
            data = getMachineProfile(ser)
            print_ctypes_obj(data)
            profile = MachineProfile()
            #setMachineProfile(ser, profile)
        except ValueError as error:
            print(error)
        time.sleep(3)