from ctypes import *
MAX_TOKENS = 200
MAX_MACHINE_PROFILE_NAME = 20
MAX_PATH_LENGTH = 50
MAX_CONFIGURATION_MOTOR_TYPE = 20
MAX_CONFIGURATION_ENCODER_TYPE = 20
MAX_CONFIGURATION_FORCE_GAUGE = 20
MAX_MOTION_PROFILE_NAME = MAX_PATH_LENGTH
MAX_MOTION_PROFILE_SETS = 6
MAX_SAMPLE_PROFILE_NAME = MAX_PATH_LENGTH
MAX_TEST_PROFILE_NAME = MAX_PATH_LENGTH
MAX_TEST_PROFILE_MP_FILENAME = 20
MAX_TEST_PROFILE_COMMENT = 256
MAX_MOTION_PROFILE_SET_NAME = MAX_PATH_LENGTH
MAX_MOTION_PROFILE_SET_TYPE = 10
MAX_MOTION_QUARTET_NAME = MAX_PATH_LENGTH
MAX_MOTION_QUARTET_PARAMETERS = 10
MAX_MOTION_QUARTETS = 6
MACHINE_CONFIGURATION_FIELD_COUNT = 1
MAX_FILE_SIZE = 1000
MACHINE_PERFORMANCE_FIELD_COUNT = 7
MACHINE_PROFILE_FIELD_COUNT = MACHINE_PERFORMANCE_FIELD_COUNT + MACHINE_CONFIGURATION_FIELD_COUNT + 2
MOTION_QUARTET_FIELD_COUNT = 4 + 10
MOTION_SET_FIELD_COUNT = 5 + 10
MOTION_PROFILE_FIELD_COUNT = 4 + 20
SAMPLE_PROFILE_FIELD_COUNT = 9
TEST_PROFILE_FIELD_COUNT = 6
class Function(c_int):
	enum = {
		0: "LINE",
		1: "SIGMOID",
	}
class MachineConfiguration(Structure):
    _fields_ = [
        ("motorType", c_char * MAX_CONFIGURATION_MOTOR_TYPE),
        ("maxMotorTorque", c_float),
        ("maxMotorRPM", c_float),
        ("gearDiameter", c_float),
        ("gearPitch", c_float),
        ("systemIntertia", c_float),
        ("staticTorque", c_float),
        ("load", c_float),
        ("positionEncoderType", c_char * MAX_CONFIGURATION_ENCODER_TYPE),
        ("positionEncoderStepsPerRev", c_float),
        ("forceGauge", c_char * MAX_CONFIGURATION_FORCE_GAUGE),
        ("forceGaugeScaleFactor", c_float),
        ("forceGaugeZeroFactor", c_int),
    ]

class MachinePerformance(Structure):
    _fields_ = [
        ("minPosition", c_float),
        ("maxPosition", c_float),
        ("maxVelocity", c_float),
        ("maxAcceleration", c_float),
        ("maxForceTensile", c_float),
        ("maxForceCompression", c_float),
        ("forceGaugeNeutralOffset", c_float),
    ]

class MachineProfile(Structure):
    _fields_ = [
        ("name", c_char * MAX_MACHINE_PROFILE_NAME),
        ("configuration", MachineConfiguration),
        ("performance", MachinePerformance),
    ]

class FunctionInfo(Structure):
    _fields_ = [
        ("function", Function),
        ("parameters", c_float * MAX_MOTION_QUARTET_PARAMETERS),
    ]

class MotionQuartet(Structure):
    _fields_ = [
        ("name", c_char * MAX_MOTION_QUARTET_NAME),
        ("function", FunctionInfo),
        ("dwell", c_float),
    ]

class MotionSet(Structure):
    _fields_ = [
        ("name", c_char * MAX_MOTION_PROFILE_SET_NAME),
        ("type", c_char * MAX_MOTION_PROFILE_SET_TYPE),
        ("executions", c_int),
        ("quartetCount", c_int),
        ("quartets", MotionQuartet * MAX_MOTION_QUARTETS),
    ]

class MotionProfile(Structure):
    _fields_ = [
        ("name", c_char * MAX_MOTION_PROFILE_NAME),
        ("setCount", c_int),
        ("sets", MotionSet * MAX_MOTION_PROFILE_SETS),
    ]

class SampleProfile(Structure):
    _fields_ = [
        ("name", c_char * MAX_SAMPLE_PROFILE_NAME),
        ("length", c_float),
        ("stretchMax", c_float),
        ("maxVelocity", c_float),
        ("maxAcceleration", c_float),
        ("maxJerk", c_float),
        ("maxForceTensile", c_float),
        ("maxForceCompression", c_float),
    ]

class TestProfile(Structure):
    _fields_ = [
        ("name", c_char * MAX_TEST_PROFILE_NAME),
        ("sampleSN", c_int),
        ("machineProfile", MachineProfile),
        ("sampleProfile", SampleProfile),
        ("motionProfile", MotionProfile),
        ("comment", c_char * MAX_TEST_PROFILE_COMMENT),
    ]

