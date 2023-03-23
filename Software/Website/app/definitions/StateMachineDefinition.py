from ctypes import Structure, c_int, c_float, c_char, c_bool
from wtforms import BooleanField,IntegerField, StringField,FloatField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
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

    def setdict(self, d):
        self.chargePump = bool(d["chargePump"])
        

    def getdict(self):
        return {
            "chargePump": self.chargePump,
        }

class SelfCheckParametersForm(FlaskForm):
    chargePump = BooleanField("chargePump",validators=[validators.DataRequired()])

class MachineCheckParameters(Structure):
    _fields_ = [
        ("switchedPower", c_bool),
        ("esdTravelLimit", c_int),
        ("esdSwitch", c_bool),
        ("servoOK", c_bool),
        ("forceGaugeCom", c_bool),
        ("servoCom", c_bool),
        ("rtcCom", c_bool),
    ]

    def setdict(self, d):
        self.switchedPower = bool(d["switchedPower"])
        self.esdTravelLimit = int(d["esdTravelLimit"])
        self.esdSwitch = bool(d["esdSwitch"])
        self.servoOK = bool(d["servoOK"])
        self.forceGaugeCom = bool(d["forceGaugeCom"])
        self.servoCom = bool(d["servoCom"])
        self.rtcCom = bool(d["rtcCom"])
        

    def getdict(self):
        return {
            "switchedPower": self.switchedPower,
            "esdTravelLimit": self.esdTravelLimit,
            "esdSwitch": self.esdSwitch,
            "servoOK": self.servoOK,
            "forceGaugeCom": self.forceGaugeCom,
            "servoCom": self.servoCom,
            "rtcCom": self.rtcCom,
        }

class MachineCheckParametersForm(FlaskForm):
    switchedPower = BooleanField("switchedPower",validators=[validators.DataRequired()])
    esdTravelLimit = IntegerField("esdTravelLimit",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    esdSwitch = BooleanField("esdSwitch",validators=[validators.DataRequired()])
    servoOK = BooleanField("servoOK",validators=[validators.DataRequired()])
    forceGaugeCom = BooleanField("forceGaugeCom",validators=[validators.DataRequired()])
    servoCom = BooleanField("servoCom",validators=[validators.DataRequired()])
    rtcCom = BooleanField("rtcCom",validators=[validators.DataRequired()])

class MotionParameters(Structure):
    _fields_ = [
        ("status", c_int),
        ("condition", c_int),
        ("mode", c_int),
    ]

    def setdict(self, d):
        self.status = int(d["status"])
        self.condition = int(d["condition"])
        self.mode = int(d["mode"])
        

    def getdict(self):
        return {
            "status": self.status,
            "condition": self.condition,
            "mode": self.mode,
        }

class MotionParametersForm(FlaskForm):
    status = IntegerField("status",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    condition = IntegerField("condition",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    mode = IntegerField("mode",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])

class MachineState(Structure):
    _fields_ = [
        ("state", c_int),
        ("selfCheckParameters", SelfCheckParameters),
        ("machineCheckParameters", MachineCheckParameters),
        ("motionParameters", MotionParameters),
        ("_function", c_int),
        ("_functionData", c_int),
        ("_lock", c_int),
    ]

    def setdict(self, d):
        self.state = int(d["state"])
        self.selfCheckParameters.setdict(d["selfCheckParameters"])
        self.machineCheckParameters.setdict(d["machineCheckParameters"])
        self.motionParameters.setdict(d["motionParameters"])
        self._function = int(d["_function"])
        self._functionData = int(d["_functionData"])
        self._lock = int(d["_lock"])
        

    def getdict(self):
        return {
            "state": self.state,
            "selfCheckParameters": self.selfCheckParameters.getdict(),
            "machineCheckParameters": self.machineCheckParameters.getdict(),
            "motionParameters": self.motionParameters.getdict(),
            "_function": self._function,
            "_functionData": self._functionData,
            "_lock": self._lock,
        }

class MachineStateForm(FlaskForm):
    state = IntegerField("state",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    selfCheckParameters = FormField(SelfCheckParametersForm)
    machineCheckParameters = FormField(MachineCheckParametersForm)
    motionParameters = FormField(MotionParametersForm)
    _function = IntegerField("_function",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    _functionData = IntegerField("_functionData",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    _lock = IntegerField("_lock",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])

