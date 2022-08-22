MAX_MACHINE_PROFILE_NAME =20
MAX_PATH_LENGTH =50
MAX_CONFIGURATION_MOTOR_TYPE =20
MAX_CONFIGURATION_ENCODER_TYPE =20
MAX_CONFIGURATION_FORCE_GAUGE =20
MAX_MOTION_PROFILE_NAME =MAX_PATH_LENGTH
MAX_MOTION_PROFILE_SETS =6
MAX_SAMPLE_PROFILE_NAME =MAX_PATH_LENGTH
MAX_TEST_PROFILE_NAME =MAX_PATH_LENGTH
MAX_TEST_PROFILE_MP_FILENAME =20
MAX_TEST_PROFILE_COMMENT =256
MAX_MOTION_PROFILE_SET_NAME =MAX_PATH_LENGTH
MAX_MOTION_PROFILE_SET_TYPE =10
MAX_MOTION_QUARTET_NAME =MAX_PATH_LENGTH
MAX_MOTION_QUARTET_PARAMETERS =10
MAX_MOTION_QUARTETS =6
MACHINE_CONFIGURATION_FIELD_COUNT =1

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

class MachineConfiguration(Structure):
    _fields_ = [
        ("motorType", ctypes.c_char*MAX_CONFIGURATION_MOTOR_TYPE),
        ("maxMotorTorque", ctypes.c_float),
        ("maxMotorRPM", ctypes.c_float),
        ("gearDiameter", ctypes.c_float),
        ("gearPitch", ctypes.c_float),
        ("systemIntertia", ctypes.c_float),
        ("staticTorque", ctypes.c_float),
        ("load", ctypes.c_float),
        ("positionEncoderType", ctypes.c_char*MAX_CONFIGURATION_ENCODER_TYPE),
        ("positionEncoderStepsPerRev", ctypes.c_float),
        ("forceGauge", ctypes.c_char*MAX_CONFIGURATION_FORCE_GAUGE),
        ("forceGaugeScaleFactor", ctypes.c_float),
        ("forceGaugeZeroFactor", ctypes.c_int)]

class MachinePerformance(Structure):
    _fields_ = [
        ("minPosition", ctypes.c_float),
        ("maxPosition", ctypes.c_float),
        ("maxVelocity", ctypes.c_float),
        ("maxAcceleration", ctypes.c_float),
        ("maxForceTensile", ctypes.c_float),
        ("maxForceCompression", ctypes.c_float),
        ("forceGaugeNeutralOffset", ctypes.c_float)]

class MachineProfile(Structure):
    _fields_ = [
        ("name", ctypes.c_char*MAX_MACHINE_PROFILE_NAME),
        ("number", ctypes.c_int32),
        ("configuration", MachineConfiguration),
        ("performance", MachinePerformance)]

class MonitorData(Structure):
    _fields_ = [
        ("forceRaw", ctypes.c_int32),
        ("encoderRaw", ctypes.c_int32),
        ("force", ctypes.c_float),
        ("position", ctypes.c_float),
        ("timems", ctypes.c_int32),
        ("timeus", ctypes.c_int32)]
