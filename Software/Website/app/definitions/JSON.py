from ctypes import Structure, c_int, c_float, c_char, c_bool
from wtforms import BooleanField,IntegerField, StringField,FloatField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
MAX_MACHINE_PROFILE_NAME = 20
MAX_PATH_LENGTH = 50
MAX_CONFIGURATION_MOTOR_TYPE = 20
MAX_CONFIGURATION_ENCODER_TYPE = 20
MAX_CONFIGURATION_FORCE_GAUGE = 20
MAX_SAMPLE_PROFILE_NAME = MAX_PATH_LENGTH
MAX_TEST_PROFILE_NAME = MAX_PATH_LENGTH
MAX_TEST_PROFILE_MP_FILENAME = 20
MAX_TEST_PROFILE_COMMENT = 256
MAX_FILE_SIZE = 1000
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
    maxMotorTorque = FloatField("maxMotorTorque",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxMotorRPM = FloatField("maxMotorRPM",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    gearDiameter = FloatField("gearDiameter",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    gearPitch = FloatField("gearPitch",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    systemIntertia = FloatField("systemIntertia",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    staticTorque = FloatField("staticTorque",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    load = FloatField("load",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    positionEncoderType = StringField("positionEncoderType", validators=[validators.Length(max=MAX_CONFIGURATION_ENCODER_TYPE)])
    positionEncoderStepsPerRev = FloatField("positionEncoderStepsPerRev",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    forceGauge = StringField("forceGauge", validators=[validators.Length(max=MAX_CONFIGURATION_FORCE_GAUGE)])
    forceGaugeScaleFactor = FloatField("forceGaugeScaleFactor",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    forceGaugeZeroFactor = IntegerField("forceGaugeZeroFactor",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])

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
    minPosition = FloatField("minPosition",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxPosition = FloatField("maxPosition",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxVelocity = FloatField("maxVelocity",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxAcceleration = FloatField("maxAcceleration",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceTensile = FloatField("maxForceTensile",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceCompression = FloatField("maxForceCompression",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    forceGaugeNeutralOffset = FloatField("forceGaugeNeutralOffset",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])

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
    length = FloatField("length",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    stretchMax = FloatField("stretchMax",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxVelocity = FloatField("maxVelocity",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxAcceleration = FloatField("maxAcceleration",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxJerk = FloatField("maxJerk",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceTensile = FloatField("maxForceTensile",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    maxForceCompression = FloatField("maxForceCompression",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])

