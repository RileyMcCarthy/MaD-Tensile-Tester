import re
import ctypes
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
    motionProfile.number = 5
    motionProfile.setCount = 1
    motionProfile.sets[0].name = b'Test set 1'
    motionProfile.sets[0].number = 1
    motionProfile.sets[0].executions = 2
    motionProfile.sets[0].quartetCount = 2

    motionProfile.sets[0].quartets[0].name = b'qrt1.qrt'
    motionProfile.sets[0].quartets[1].function.id = 0
    motionProfile.sets[0].quartets[1].function.parameters[0] = -20
    motionProfile.sets[0].quartets[1].function.parameters[1] = 10
    motionProfile.sets[0].quartets[0].dwell = 500
    print_ctypes_obj(motionProfile.sets[0].quartets[1].function)

    motionProfile.sets[0].quartets[1].name = b'qrt2.qrt'
    motionProfile.sets[0].quartets[1].function.id = 0
    motionProfile.sets[0].quartets[1].function.parameters[0] = 20
    motionProfile.sets[0].quartets[1].function.parameters[1] = -10
    motionProfile.sets[0].quartets[1].dwell = 500
    return motionProfile


def loadTestProfile():
    testProfile = TestProfile()
    testProfile.name = b'Test_Profile_1'
    testProfile.machineProfile = loadMachineProfile()
    testProfile.motionProfile = loadMotionProfile()
    testProfile.comment = "Demo test profile"
    return testProfile


def loadMachineProfile():
    machineProfile = MachineProfile()
    machineProfile.name = b'Tensile_Test_1'

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

def ctypes_to_dict(ctypes_obj):
    def convert(obj):
        if isinstance(obj, ctypes.Structure):
            return ctypes_to_dict(obj)
        elif isinstance(obj, ctypes.Array):
            return [convert(elem) for elem in obj]
        elif isinstance(obj, ctypes.c_int) and hasattr(obj, 'enum'):
            return obj.enum[obj.value]
        else:
            return obj

    return {field[0]: convert(getattr(ctypes_obj, field[0])) for field in ctypes_obj._fields_}
