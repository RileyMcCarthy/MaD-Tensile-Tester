r"""Wrapper for JSON.h

Generated with:
/home/riley/.local/bin/ctypesgen MaD_Firmware/Librarys/JSON/JSON.h -o MaD_Software/JSON.py --no-embed-preamble

Do not modify this file.
"""

__docformat__ = "restructuredtext"

# Begin preamble for Python

from ctypes_preamble import *
from ctypes_preamble import _variadic_function

# End preamble

_libs = {}
_libdirs = []

# Begin loader

from ctypes_loader import *

# End loader

add_library_search_dirs([])

# No libraries

# No modules

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 48
class struct_MachineConfiguration_s(Structure):
    pass

struct_MachineConfiguration_s.__slots__ = [
    'motorType',
    'maxMotorTorque',
    'maxMotorRPM',
    'gearDiameter',
    'gearPitch',
    'systemIntertia',
    'staticTorque',
    'load',
    'positionEncoderType',
    'positionEncoderStepsPerRev',
    'forceGauge',
    'forceGaugeScaleFactor',
    'forceGaugeZeroFactor',
]
struct_MachineConfiguration_s._fields_ = [
    ('motorType', c_char * int(20)),
    ('maxMotorTorque', c_float),
    ('maxMotorRPM', c_float),
    ('gearDiameter', c_float),
    ('gearPitch', c_float),
    ('systemIntertia', c_float),
    ('staticTorque', c_float),
    ('load', c_float),
    ('positionEncoderType', c_char * int(20)),
    ('positionEncoderStepsPerRev', c_float),
    ('forceGauge', c_char * int(20)),
    ('forceGaugeScaleFactor', c_float),
    ('forceGaugeZeroFactor', c_int),
]

MachineConfiguration = struct_MachineConfiguration_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 48

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 60
class struct_MachinePerformance_s(Structure):
    pass

struct_MachinePerformance_s.__slots__ = [
    'minPosition',
    'maxPosition',
    'maxVelocity',
    'maxAcceleration',
    'maxForceTensile',
    'maxForceCompression',
    'forceGaugeNeutralOffset',
]
struct_MachinePerformance_s._fields_ = [
    ('minPosition', c_float),
    ('maxPosition', c_float),
    ('maxVelocity', c_float),
    ('maxAcceleration', c_float),
    ('maxForceTensile', c_float),
    ('maxForceCompression', c_float),
    ('forceGaugeNeutralOffset', c_float),
]

MachinePerformance = struct_MachinePerformance_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 60

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 69
class struct_MachineProfile_s(Structure):
    pass

struct_MachineProfile_s.__slots__ = [
    'name',
    'number',
    'configuration',
    'performance',
]
struct_MachineProfile_s._fields_ = [
    ('name', c_char * int(20)),
    ('number', c_int),
    ('configuration', MachineConfiguration),
    ('performance', MachinePerformance),
]

MachineProfile = struct_MachineProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 69

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 80
class struct_MotionQuartet_s(Structure):
    pass

struct_MotionQuartet_s.__slots__ = [
    'name',
    'function',
    'parameters',
    'dwell',
]
struct_MotionQuartet_s._fields_ = [
    ('name', c_char * int(50)),
    ('function', c_int),
    ('parameters', c_float * int(10)),
    ('dwell', c_float),
]

MotionQuartet = struct_MotionQuartet_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 80

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 91
class struct_MotionSet_s(Structure):
    pass

struct_MotionSet_s.__slots__ = [
    'name',
    'number',
    'type',
    'executions',
    'quartetCount',
    'quartets',
]
struct_MotionSet_s._fields_ = [
    ('name', c_char * int(50)),
    ('number', c_int),
    ('type', c_char * int(10)),
    ('executions', c_int),
    ('quartetCount', c_int),
    ('quartets', MotionQuartet * int(6)),
]

MotionSet = struct_MotionSet_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 91

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 100
class struct_MotionProfile_s(Structure):
    pass

struct_MotionProfile_s.__slots__ = [
    'name',
    'number',
    'setCount',
    'sets',
]
struct_MotionProfile_s._fields_ = [
    ('name', c_char * int(50)),
    ('number', c_int),
    ('setCount', c_int),
    ('sets', MotionSet * int(6)),
]

MotionProfile = struct_MotionProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 100

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 115
class struct_SampleProfile_s(Structure):
    pass

struct_SampleProfile_s.__slots__ = [
    'name',
    'number',
    'length',
    'stretchMax',
    'maxVelocity',
    'maxAcceleration',
    'maxJerk',
    'maxForceTensile',
    'maxForceCompression',
]
struct_SampleProfile_s._fields_ = [
    ('name', c_char * int(50)),
    ('number', c_int),
    ('length', c_float),
    ('stretchMax', c_float),
    ('maxVelocity', c_float),
    ('maxAcceleration', c_float),
    ('maxJerk', c_float),
    ('maxForceTensile', c_float),
    ('maxForceCompression', c_float),
]

SampleProfile = struct_SampleProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 115

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 126
class struct_TestProfile_s(Structure):
    pass

struct_TestProfile_s.__slots__ = [
    'name',
    'sampleSN',
    'machineProfile',
    'sampleProfile',
    'motionProfile',
    'comment',
]
struct_TestProfile_s._fields_ = [
    ('name', c_char * int(50)),
    ('sampleSN', c_int),
    ('machineProfile', MachineProfile),
    ('sampleProfile', SampleProfile),
    ('motionProfile', MotionProfile),
    ('comment', c_char * int(256)),
]

TestProfile = struct_TestProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 126

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 4
try:
    MAX_TOKENS = 200
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 5
try:
    MAX_MACHINE_PROFILE_NAME = 20
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 7
try:
    MAX_PATH_LENGTH = 50
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 9
try:
    MAX_CONFIGURATION_MOTOR_TYPE = 20
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 10
try:
    MAX_CONFIGURATION_ENCODER_TYPE = 20
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 11
try:
    MAX_CONFIGURATION_FORCE_GAUGE = 20
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 13
try:
    MAX_MOTION_PROFILE_NAME = MAX_PATH_LENGTH
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 14
try:
    MAX_MOTION_PROFILE_SETS = 6
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 16
try:
    MAX_SAMPLE_PROFILE_NAME = MAX_PATH_LENGTH
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 18
try:
    MAX_TEST_PROFILE_NAME = MAX_PATH_LENGTH
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 19
try:
    MAX_TEST_PROFILE_MP_FILENAME = 20
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 20
try:
    MAX_TEST_PROFILE_COMMENT = 256
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 22
try:
    MAX_MOTION_PROFILE_SET_NAME = MAX_PATH_LENGTH
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 23
try:
    MAX_MOTION_PROFILE_SET_TYPE = 10
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 25
try:
    MAX_MOTION_QUARTET_NAME = MAX_PATH_LENGTH
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 26
try:
    MAX_MOTION_QUARTET_PARAMETERS = 10
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 27
try:
    MAX_MOTION_QUARTETS = 6
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 29
try:
    MACHINE_CONFIGURATION_FIELD_COUNT = 1
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 31
try:
    MAX_FILE_SIZE = 1000
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 50
try:
    MACHINE_PERFORMANCE_FIELD_COUNT = 7
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 62
try:
    MACHINE_PROFILE_FIELD_COUNT = ((MACHINE_PERFORMANCE_FIELD_COUNT + MACHINE_CONFIGURATION_FIELD_COUNT) + 2)
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 73
try:
    MOTION_QUARTET_FIELD_COUNT = (4 + 10)
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 82
try:
    MOTION_SET_FIELD_COUNT = (5 + 10)
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 93
try:
    MOTION_PROFILE_FIELD_COUNT = (4 + 20)
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 102
try:
    SAMPLE_PROFILE_FIELD_COUNT = 9
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 117
try:
    TEST_PROFILE_FIELD_COUNT = 6
except:
    pass

MachineConfiguration_s = struct_MachineConfiguration_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 48

MachinePerformance_s = struct_MachinePerformance_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 60

MachineProfile_s = struct_MachineProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 69

MotionQuartet_s = struct_MotionQuartet_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 80

MotionSet_s = struct_MotionSet_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 91

MotionProfile_s = struct_MotionProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 100

SampleProfile_s = struct_SampleProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 115

TestProfile_s = struct_TestProfile_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON/JSON.h: 126

# No inserted files

# No prefix-stripping

