import re
import ctypes
from .definitions.JSON import MachineProfile
from collections.abc import MutableMapping

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

#def ctypes_to_dict(ctypes_obj):
##    def convert(obj):
#        if issubclass(obj, ctypes.Structure):
#            return ctypes_to_dict(obj)
#        elif issubclass(obj, ctypes.Array):
#            return [convert(elem) for elem in obj]
#        elif issubclass(obj, ctypes.c_int) and hasattr(obj, 'enum'):
#            return {"enum": {"type":"{}".format(obj.__class__.__name__),"value": obj.enum[obj.value], "options": [obj.enum[e] for e in obj.enum]}}
#        else:
#            print(obj)
#            print(type(obj))
#            return obj.value

#    return {field_name: convert(getattr(ctypes_obj, field_name)) for field_name, field_type in ctypes_obj._fields_}

def ctypes_to_dict(ctypes_structure):
    result = {}
    for field_name, field_type in ctypes_structure._fields_:
        value = getattr(ctypes_structure, field_name)
        print(field_name+"("+str(field_type)+")"+" = "+str(value)+"("+str(type(value))+")")
        if issubclass(field_type, ctypes.Structure):
            result[field_name] = ctypes_to_dict(value)
        elif issubclass(field_type, ctypes.Array):
            result[field_name] = list(value)
        else:
            result[field_name] = value
    return result

def dict_to_ctypes(ctypes_struct_class, data):
    obj = ctypes_struct_class()
    for field_name, field_type in obj._fields_:
        value = data[field_name]
        print(field_name+"("+str(field_type)+")"+" = "+str(value)+"("+str(type(value))+")")
        if issubclass(field_type, ctypes.Array):
            setattr(obj, field_name, field_type(*value))
        elif issubclass(field_type, ctypes.Structure):
            setattr(obj, field_name, dict_to_ctypes(field_type, value))
        elif hasattr(obj, 'enum'):
            setattr(obj, field_name, field_type(field_type.enum[value["enum"]["value"]]))
        else:
            setattr(obj, field_name, field_type(value))
    return obj

def unflatten_dict(d):
    result = {}
    for key, value in d.items():
        parts = key.split('-')
        d = result
        for part in parts[:-1]:
            if part not in d:
                d[part] = {}
            d = d[part]
        d[parts[-1]] = value
    return result

def flatten_dict(d, parent_key='', sep='-'):
    items = []
    for k, v in d.items():
        new_key = parent_key + sep + k if parent_key else k
        if isinstance(v, MutableMapping):
            items.extend(flatten_dict(v, new_key, sep=sep).items())
        else:
            items.append((new_key, v))
    return dict(items)