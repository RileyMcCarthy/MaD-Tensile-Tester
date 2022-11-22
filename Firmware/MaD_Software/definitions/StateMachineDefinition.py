r"""Wrapper for StateMachineDefinition.h

Generated with:
/home/mad/.local/bin/ctypesgen MaD_Firmware/include/Utility/StateMachineDefinition.h -o MaD_Software/definitions/StateMachineDefinition.py --no-embed-preamble

Do not modify this file.
"""

__docformat__ = "restructuredtext"

# Begin preamble for Python

from .ctypes_preamble import *
from .ctypes_preamble import _variadic_function

# End preamble

_libs = {}
_libdirs = []

# Begin loader

from .ctypes_loader import *

# End loader

add_library_search_dirs([])

# No libraries

# No modules

enum_States_e = c_int# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 11

STATE_SELFCHECK = 0# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 11

STATE_MACHINECHECK = (STATE_SELFCHECK + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 11

STATE_MOTION = (STATE_MACHINECHECK + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 11

State = enum_States_e# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 11

enum_MotionStatus_e = c_int# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 20

MOTIONSTATUS_DISABLED = 0# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 20

MOTIONSTATUS_ENABLED = (MOTIONSTATUS_DISABLED + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 20

MOTIONSTATUS_SAMPLE_LIMIT = (MOTIONSTATUS_ENABLED + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 20

MOTIONSTATUS_MACHINE_LIMIT = (MOTIONSTATUS_SAMPLE_LIMIT + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 20

MOTIONSTATUS_FAULTED = (MOTIONSTATUS_MACHINE_LIMIT + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 20

MotionStatus = enum_MotionStatus_e# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 20

enum_MotionOverTravel_e = c_int# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 27

MOTION_LIMIT_OK = 0# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 27

MOTION_LIMIT_UPPER = (MOTION_LIMIT_OK + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 27

MOTION_LIMIT_LOWER = (MOTION_LIMIT_UPPER + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 27

MotionOverTravel = enum_MotionOverTravel_e# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 27

enum_MotionCondition_e = c_int# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_LENGTH = 0# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_FORCE = (CONDITION_LENGTH + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_TENSION = (CONDITION_FORCE + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_COMPRESSION = (CONDITION_TENSION + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_UPPER = (CONDITION_COMPRESSION + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_LOWER = (CONDITION_UPPER + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_DOOR = (CONDITION_LOWER + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_STOPPED = (CONDITION_DOOR + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

CONDITION_MOVING = (CONDITION_STOPPED + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

MotionCondition = enum_MotionCondition_e# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 44

enum_MotionMode_e = c_int# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 51

MODE_MANUAL = 0# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 51

MODE_TEST = (MODE_MANUAL + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 51

MODE_TEST_RUNNING = (MODE_TEST + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 51

MotionMode = enum_MotionMode_e# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 51

enum_ModeFunctions_e = c_int# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_MANUAL_OFF = 0# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_MANUAL_INCREMENTAL_JOG = (FUNC_MANUAL_OFF + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_MANUAL_CONTINUOUS_JOG = (FUNC_MANUAL_INCREMENTAL_JOG + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_MANUAL_POSITIONAL_MOVE = (FUNC_MANUAL_CONTINUOUS_JOG + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_MANUAL_HOME = (FUNC_MANUAL_POSITIONAL_MOVE + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_MANUAL_MOVE_GAUGE_LENGTH = (FUNC_MANUAL_HOME + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_MANUAL_MOVE_FORCE = (FUNC_MANUAL_MOVE_GAUGE_LENGTH + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_TEST_LOAD = (FUNC_MANUAL_MOVE_FORCE + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_TEST_RUN = (FUNC_TEST_LOAD + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_TEST_STOP = (FUNC_TEST_RUN + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

FUNC_TEST_TOGGLE_HOLD_RESUME = (FUNC_TEST_STOP + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

ModeFunctions = enum_ModeFunctions_e# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 66

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 71
class struct_SelfCheck_t(Structure):
    pass

struct_SelfCheck_t.__slots__ = [
    'chargePump',
]
struct_SelfCheck_t._fields_ = [
    ('chargePump', c_bool),
]

SelfCheckParameters = struct_SelfCheck_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 71

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 83
class struct_MachineCheck_t(Structure):
    pass

struct_MachineCheck_t.__slots__ = [
    'switchedPower',
    'esdTravelLimit',
    'esdSwitch',
    'servoOK',
    'forceGaugeCom',
    'servoCom',
    'rtcCom',
]
struct_MachineCheck_t._fields_ = [
    ('switchedPower', c_bool),
    ('esdTravelLimit', MotionOverTravel),
    ('esdSwitch', c_bool),
    ('servoOK', c_bool),
    ('forceGaugeCom', c_bool),
    ('servoCom', c_bool),
    ('rtcCom', c_bool),
]

MachineCheckParameters = struct_MachineCheck_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 83

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 90
class struct_Motion_t(Structure):
    pass

struct_Motion_t.__slots__ = [
    'status',
    'condition',
    'mode',
]
struct_Motion_t._fields_ = [
    ('status', MotionStatus),
    ('condition', MotionCondition),
    ('mode', MotionMode),
]

MotionParameters = struct_Motion_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 90

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 102
class struct_MachineState_t(Structure):
    pass

struct_MachineState_t.__slots__ = [
    'state',
    'selfCheckParameters',
    'machineCheckParameters',
    'motionParameters',
    '_function',
    '_functionData',
    '_lock',
]
struct_MachineState_t._fields_ = [
    ('state', State),
    ('selfCheckParameters', SelfCheckParameters),
    ('machineCheckParameters', MachineCheckParameters),
    ('motionParameters', MotionParameters),
    ('_function', ModeFunctions),
    ('_functionData', c_int),
    ('_lock', c_int),
]

MachineState = struct_MachineState_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 102

enum_parameters_e = c_int# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_SELF_CHARGE_PUMP = 0# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MACHINE_SWITCHED_POWER = (PARAM_SELF_CHARGE_PUMP + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MACHINE_ESD_TRAVEL_LIMIT = (PARAM_MACHINE_SWITCHED_POWER + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MACHINE_ESD_SWITCH = (PARAM_MACHINE_ESD_TRAVEL_LIMIT + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MACHINE_SERVO_OK = (PARAM_MACHINE_ESD_SWITCH + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MACHINE_FORCE_GAUGE_COM = (PARAM_MACHINE_SERVO_OK + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MACHINE_SERVO_COM = (PARAM_MACHINE_FORCE_GAUGE_COM + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MACHINE_RTC_COM = (PARAM_MACHINE_SERVO_COM + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MOTION_STATUS = (PARAM_MACHINE_RTC_COM + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MOTION_CONDITION = (PARAM_MOTION_STATUS + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_MOTION_MODE = (PARAM_MOTION_CONDITION + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

PARAM_FUNCTION = (PARAM_MOTION_MODE + 1)# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

Parameter = enum_parameters_e# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 118

SelfCheck_t = struct_SelfCheck_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 71

MachineCheck_t = struct_MachineCheck_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 83

Motion_t = struct_Motion_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 90

MachineState_t = struct_MachineState_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/StateMachineDefinition.h: 102

# No inserted files

# No prefix-stripping

