from ctypes import *
class State(c_int):
    pass

State.SELFCHECK = 0
State.MACHINECHECK = 1
State.MOTION = 2

class MotionStatus(c_int):
    pass

MotionStatus.DISABLED = 0
MotionStatus.ENABLED = 1
MotionStatus.SAMPLE_LIMIT = 2
MotionStatus.MACHINE_LIMIT = 3
MotionStatus.FAULTED = 4

class MotionOverTravel(c_int):
    pass

MotionOverTravel.MOTION_LIMIT_OK = 0
MotionOverTravel.MOTION_LIMIT_UPPER = 1
MotionOverTravel.MOTION_LIMIT_LOWER = 2

class MotionCondition(c_int):
    pass

MotionCondition.CONDITION_LENGTH = 0
MotionCondition.CONDITION_FORCE = 1
MotionCondition.CONDITION_TENSION = 2
MotionCondition.CONDITION_COMPRESSION = 3
MotionCondition.CONDITION_UPPER = 4
MotionCondition.CONDITION_LOWER = 5
MotionCondition.CONDITION_DOOR = 6
MotionCondition.CONDITION_STOPPED = 7
MotionCondition.CONDITION_MOVING = 8

class MotionMode(c_int):
    pass

MotionMode.MODE_MANUAL = 0
MotionMode.MODE_TEST = 1
MotionMode.MODE_TEST_RUNNING = 2

class ModeFunctions(c_int):
    pass

ModeFunctions.FUNC_MANUAL_OFF = 0
ModeFunctions.FUNC_MANUAL_INCREMENTAL_JOG = 1
ModeFunctions.FUNC_MANUAL_CONTINUOUS_JOG = 2
ModeFunctions.FUNC_MANUAL_POSITIONAL_MOVE = 3
ModeFunctions.FUNC_MANUAL_HOME = 4
ModeFunctions.FUNC_MANUAL_MOVE_GAUGE_LENGTH = 5
ModeFunctions.FUNC_MANUAL_MOVE_FORCE = 6
ModeFunctions.FUNC_TEST_LOAD = 7
ModeFunctions.FUNC_TEST_RUN = 8
ModeFunctions.FUNC_TEST_STOP = 9
ModeFunctions.FUNC_TEST_TOGGLE_HOLD_RESUME = 10

class Parameter(c_int):
    pass

Parameter.PARAM_SELF_CHARGE_PUMP = 0
Parameter.PARAM_MACHINE_SWITCHED_POWER = 1
Parameter.PARAM_MACHINE_ESD_TRAVEL_LIMIT = 2
Parameter.PARAM_MACHINE_ESD_SWITCH = 3
Parameter.PARAM_MACHINE_SERVO_OK = 4
Parameter.PARAM_MACHINE_FORCE_GAUGE_COM = 5
Parameter.PARAM_MACHINE_SERVO_COM = 6
Parameter.PARAM_MACHINE_RTC_COM = 7
Parameter.PARAM_MOTION_STATUS = 8
Parameter.PARAM_MOTION_CONDITION = 9
Parameter.PARAM_MOTION_MODE = 10
Parameter.PARAM_FUNCTION = 11

class SelfCheckParameters(Structure):
    _fields_ = [
        ("chargePump", c_bool),
    ]

class MachineCheckParameters(Structure):
    _fields_ = [
        ("switchedPower", c_bool),
        ("esdTravelLimit", MotionOverTravel),
        ("esdSwitch", c_bool),
        ("servoOK", c_bool),
        ("forceGaugeCom", c_bool),
        ("servoCom", c_bool),
        ("rtcCom", c_bool),
    ]

class MotionParameters(Structure):
    _fields_ = [
        ("status", MotionStatus),
        ("condition", MotionCondition),
        ("mode", MotionMode),
    ]

class MachineState(Structure):
    _fields_ = [
        ("state", State),
        ("selfCheckParameters", SelfCheckParameters),
        ("machineCheckParameters", MachineCheckParameters),
        ("motionParameters", MotionParameters),
        ("_function", ModeFunctions),
        ("_functionData", c_int),
        ("_lock", c_int),
    ]

