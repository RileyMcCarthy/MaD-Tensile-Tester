from ctypes import Structure, c_int, c_float, c_char, c_bool
from wtforms import BooleanField,IntegerField, StringField,FloatField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
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

    def setdict(self, d):
        self.motorType = str(d["motorType"]).encode("utf-8")
        self.maxMotorTorque = float(d["maxMotorTorque"])
        self.maxMotorRPM = float(d["maxMotorRPM"])
        self.gearDiameter = float(d["gearDiameter"])
        self.gearPitch = float(d["gearPitch"])
        self.systemIntertia = float(d["systemIntertia"])
        self.staticTorque = float(d["staticTorque"])
        self.load = float(d["load"])
        self.positionEncoderType = str(d["positionEncoderType"]).encode("utf-8")
        self.positionEncoderStepsPerRev = float(d["positionEncoderStepsPerRev"])
        self.forceGauge = str(d["forceGauge"]).encode("utf-8")
        self.forceGaugeScaleFactor = float(d["forceGaugeScaleFactor"])
        self.forceGaugeZeroFactor = int(d["forceGaugeZeroFactor"])
        

    def getdict(self):
        return {
            "motorType": self.motorType.decode("utf-8"),
            "maxMotorTorque": self.maxMotorTorque,
            "maxMotorRPM": self.maxMotorRPM,
            "gearDiameter": self.gearDiameter,
            "gearPitch": self.gearPitch,
            "systemIntertia": self.systemIntertia,
            "staticTorque": self.staticTorque,
            "load": self.load,
            "positionEncoderType": self.positionEncoderType.decode("utf-8"),
            "positionEncoderStepsPerRev": self.positionEncoderStepsPerRev,
            "forceGauge": self.forceGauge.decode("utf-8"),
            "forceGaugeScaleFactor": self.forceGaugeScaleFactor,
            "forceGaugeZeroFactor": self.forceGaugeZeroFactor,
        }

class MachineConfigurationForm(FlaskForm):
    motorType = StringField("motorType", validators=[validators.Length(max=MAX_CONFIGURATION_MOTOR_TYPE)])
    maxMotorTorque = FloatField("maxMotorTorque",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxMotorRPM = FloatField("maxMotorRPM",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    gearDiameter = FloatField("gearDiameter",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    gearPitch = FloatField("gearPitch",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    systemIntertia = FloatField("systemIntertia",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    staticTorque = FloatField("staticTorque",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    load = FloatField("load",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    positionEncoderType = StringField("positionEncoderType", validators=[validators.Length(max=MAX_CONFIGURATION_ENCODER_TYPE)])
    positionEncoderStepsPerRev = FloatField("positionEncoderStepsPerRev",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    forceGauge = StringField("forceGauge", validators=[validators.Length(max=MAX_CONFIGURATION_FORCE_GAUGE)])
    forceGaugeScaleFactor = FloatField("forceGaugeScaleFactor",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    forceGaugeZeroFactor = IntegerField("forceGaugeZeroFactor",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])

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

    def setdict(self, d):
        self.minPosition = float(d["minPosition"])
        self.maxPosition = float(d["maxPosition"])
        self.maxVelocity = float(d["maxVelocity"])
        self.maxAcceleration = float(d["maxAcceleration"])
        self.maxForceTensile = float(d["maxForceTensile"])
        self.maxForceCompression = float(d["maxForceCompression"])
        self.forceGaugeNeutralOffset = float(d["forceGaugeNeutralOffset"])
        

    def getdict(self):
        return {
            "minPosition": self.minPosition,
            "maxPosition": self.maxPosition,
            "maxVelocity": self.maxVelocity,
            "maxAcceleration": self.maxAcceleration,
            "maxForceTensile": self.maxForceTensile,
            "maxForceCompression": self.maxForceCompression,
            "forceGaugeNeutralOffset": self.forceGaugeNeutralOffset,
        }

class MachinePerformanceForm(FlaskForm):
    minPosition = FloatField("minPosition",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxPosition = FloatField("maxPosition",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxVelocity = FloatField("maxVelocity",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxAcceleration = FloatField("maxAcceleration",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceTensile = FloatField("maxForceTensile",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceCompression = FloatField("maxForceCompression",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    forceGaugeNeutralOffset = FloatField("forceGaugeNeutralOffset",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])

class MachineProfile(Structure):
    _fields_ = [
        ("name", c_char * MAX_MACHINE_PROFILE_NAME),
        ("configuration", MachineConfiguration),
        ("performance", MachinePerformance),
    ]

    def setdict(self, d):
        self.name = str(d["name"]).encode("utf-8")
        self.configuration.setdict(d["configuration"])
        self.performance.setdict(d["performance"])
        

    def getdict(self):
        return {
            "name": self.name.decode("utf-8"),
            "configuration": self.configuration.getdict(),
            "performance": self.performance.getdict(),
        }

class MachineProfileForm(FlaskForm):
    name = StringField("name", validators=[validators.Length(max=MAX_MACHINE_PROFILE_NAME)])
    configuration = FormField(MachineConfigurationForm)
    performance = FormField(MachinePerformanceForm)

class FunctionInfo(Structure):
    _fields_ = [
        ("function", c_int),
        ("parameters", c_float * MAX_MOTION_QUARTET_PARAMETERS),
    ]

    def setdict(self, d):
        self.function = int(d["function"])
        self.parameters = int(d["parameters"])
        

    def getdict(self):
        return {
            "function": self.function,
            "parameters": self.parameters,
        }

class FunctionInfoForm(FlaskForm):
    function = IntegerField("function",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    parameters = StringField("parameters")

class MotionQuartet(Structure):
    _fields_ = [
        ("name", c_char * MAX_MOTION_QUARTET_NAME),
        ("function", FunctionInfo),
        ("dwell", c_float),
    ]

    def setdict(self, d):
        self.name = str(d["name"]).encode("utf-8")
        self.function.setdict(d["function"])
        self.dwell = float(d["dwell"])
        

    def getdict(self):
        return {
            "name": self.name.decode("utf-8"),
            "function": self.function.getdict(),
            "dwell": self.dwell,
        }

class MotionQuartetForm(FlaskForm):
    name = StringField("name", validators=[validators.Length(max=MAX_MOTION_QUARTET_NAME)])
    function = FormField(FunctionInfoForm)
    dwell = FloatField("dwell",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])

class MotionSet(Structure):
    _fields_ = [
        ("name", c_char * MAX_MOTION_PROFILE_SET_NAME),
        ("type", c_char * MAX_MOTION_PROFILE_SET_TYPE),
        ("executions", c_int),
        ("quartetCount", c_int),
        ("quartets", MotionQuartet),
    ]

    def setdict(self, d):
        self.name = str(d["name"]).encode("utf-8")
        self.type = str(d["type"]).encode("utf-8")
        self.executions = int(d["executions"])
        self.quartetCount = int(d["quartetCount"])
        self.quartets.setdict(d["quartets"])
        

    def getdict(self):
        return {
            "name": self.name.decode("utf-8"),
            "type": self.type.decode("utf-8"),
            "executions": self.executions,
            "quartetCount": self.quartetCount,
            "quartets": self.quartets.getdict(),
        }

class MotionSetForm(FlaskForm):
    name = StringField("name", validators=[validators.Length(max=MAX_MOTION_PROFILE_SET_NAME)])
    type = StringField("type", validators=[validators.Length(max=MAX_MOTION_PROFILE_SET_TYPE)])
    executions = IntegerField("executions",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    quartetCount = IntegerField("quartetCount",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    quartets = FormField(MotionQuartetForm)

class MotionProfile(Structure):
    _fields_ = [
        ("name", c_char * MAX_MOTION_PROFILE_NAME),
        ("setCount", c_int),
        ("sets", MotionSet),
    ]

    def setdict(self, d):
        self.name = str(d["name"]).encode("utf-8")
        self.setCount = int(d["setCount"])
        self.sets.setdict(d["sets"])
        

    def getdict(self):
        return {
            "name": self.name.decode("utf-8"),
            "setCount": self.setCount,
            "sets": self.sets.getdict(),
        }

class MotionProfileForm(FlaskForm):
    name = StringField("name", validators=[validators.Length(max=MAX_MOTION_PROFILE_NAME)])
    setCount = IntegerField("setCount",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    sets = FormField(MotionSetForm)

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

    def setdict(self, d):
        self.name = str(d["name"]).encode("utf-8")
        self.length = float(d["length"])
        self.stretchMax = float(d["stretchMax"])
        self.maxVelocity = float(d["maxVelocity"])
        self.maxAcceleration = float(d["maxAcceleration"])
        self.maxJerk = float(d["maxJerk"])
        self.maxForceTensile = float(d["maxForceTensile"])
        self.maxForceCompression = float(d["maxForceCompression"])
        

    def getdict(self):
        return {
            "name": self.name.decode("utf-8"),
            "length": self.length,
            "stretchMax": self.stretchMax,
            "maxVelocity": self.maxVelocity,
            "maxAcceleration": self.maxAcceleration,
            "maxJerk": self.maxJerk,
            "maxForceTensile": self.maxForceTensile,
            "maxForceCompression": self.maxForceCompression,
        }

class SampleProfileForm(FlaskForm):
    name = StringField("name", validators=[validators.Length(max=MAX_SAMPLE_PROFILE_NAME)])
    length = FloatField("length",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    stretchMax = FloatField("stretchMax",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxVelocity = FloatField("maxVelocity",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxAcceleration = FloatField("maxAcceleration",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxJerk = FloatField("maxJerk",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceTensile = FloatField("maxForceTensile",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceCompression = FloatField("maxForceCompression",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])

class TestProfile(Structure):
    _fields_ = [
        ("name", c_char * MAX_TEST_PROFILE_NAME),
        ("sampleSN", c_int),
        ("machineProfile", MachineProfile),
        ("sampleProfile", SampleProfile),
        ("motionProfile", MotionProfile),
        ("comment", c_char * MAX_TEST_PROFILE_COMMENT),
    ]

    def setdict(self, d):
        self.name = str(d["name"]).encode("utf-8")
        self.sampleSN = int(d["sampleSN"])
        self.machineProfile.setdict(d["machineProfile"])
        self.sampleProfile.setdict(d["sampleProfile"])
        self.motionProfile.setdict(d["motionProfile"])
        self.comment = str(d["comment"]).encode("utf-8")
        

    def getdict(self):
        return {
            "name": self.name.decode("utf-8"),
            "sampleSN": self.sampleSN,
            "machineProfile": self.machineProfile.getdict(),
            "sampleProfile": self.sampleProfile.getdict(),
            "motionProfile": self.motionProfile.getdict(),
            "comment": self.comment.decode("utf-8"),
        }

class TestProfileForm(FlaskForm):
    name = StringField("name", validators=[validators.Length(max=MAX_TEST_PROFILE_NAME)])
    sampleSN = IntegerField("sampleSN",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    machineProfile = FormField(MachineProfileForm)
    sampleProfile = FormField(SampleProfileForm)
    motionProfile = FormField(MotionProfileForm)
    comment = StringField("comment", validators=[validators.Length(max=MAX_TEST_PROFILE_COMMENT)])

