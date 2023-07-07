import ctypes
from collections.abc import MutableMapping

def loadMachineProfile():
    machineProfile = dict()
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