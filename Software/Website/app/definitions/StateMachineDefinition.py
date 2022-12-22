from ctypes import *
class State(c_int):
	enum = {
		0: "SELFCHECK",
		1: "MACHINECHECK",
		2: "MOTION",
	}
class MotionStatus(c_int):
	enum = {
		0: "DISABLED",
		1: "ENABLED",
		2: "SAMPLE_LIMIT",
		3: "MACHINE_LIMIT",
		4: "FAULTED",
	}
class MotionOverTravel(c_int):
	enum = {
		0: "MOTION_LIMIT_OK",
		1: "MOTION_LIMIT_UPPER",
		2: "MOTION_LIMIT_LOWER",
	}
class MotionCondition(c_int):
	enum = {
		0: "CONDITION_LENGTH",
		1: "CONDITION_FORCE",
		2: "CONDITION_TENSION",
		3: "CONDITION_COMPRESSION",
		4: "CONDITION_UPPER",
		5: "CONDITION_LOWER",
		6: "CONDITION_DOOR",
		7: "CONDITION_STOPPED",
		8: "CONDITION_MOVING",
	}
class MotionMode(c_int):
	enum = {
		0: "MODE_MANUAL",
		1: "MODE_TEST",
		2: "MODE_TEST_RUNNING",
	}
class ModeFunctions(c_int):
	enum = {
		0: "FUNC_MANUAL_OFF",
		1: "FUNC_MANUAL_INCREMENTAL_JOG",
		2: "FUNC_MANUAL_CONTINUOUS_JOG",
		3: "FUNC_MANUAL_POSITIONAL_MOVE",
		4: "FUNC_MANUAL_HOME",
		5: "FUNC_MANUAL_MOVE_GAUGE_LENGTH",
		6: "FUNC_MANUAL_MOVE_FORCE",
		7: "FUNC_TEST_LOAD",
		8: "FUNC_TEST_RUN",
		9: "FUNC_TEST_STOP",
		10: "FUNC_TEST_TOGGLE_HOLD_RESUME",
	}
class Parameter(c_int):
	enum = {
		0: "PARAM_SELF_CHARGE_PUMP",
		1: "PARAM_MACHINE_SWITCHED_POWER",
		2: "PARAM_MACHINE_ESD_TRAVEL_LIMIT",
		3: "PARAM_MACHINE_ESD_SWITCH",
		4: "PARAM_MACHINE_SERVO_OK",
		5: "PARAM_MACHINE_FORCE_GAUGE_COM",
		6: "PARAM_MACHINE_SERVO_COM",
		7: "PARAM_MACHINE_RTC_COM",
		8: "PARAM_MOTION_STATUS",
		9: "PARAM_MOTION_CONDITION",
		10: "PARAM_MOTION_MODE",
		11: "PARAM_FUNCTION",
	}
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

