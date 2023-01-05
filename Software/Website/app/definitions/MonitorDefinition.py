from ctypes import Structure, c_int, c_float, c_char, c_bool
from wtforms import BooleanField,IntegerField, StringField,FloatField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
class monitor_data_t(Structure):
    _fields_ = [
        ("forceRaw", c_int),
        ("encoderRaw", c_int),
        ("forcemN", c_int),
        ("encoderum", c_int),
        ("force", c_float),
        ("position", c_float),
        ("setpoint", c_int),
        ("timems", c_int),
        ("timeus", c_int),
        ("log", c_int),
    ]

    def setdict(self, d):
        self.forceRaw = int(d["forceRaw"])
        self.encoderRaw = int(d["encoderRaw"])
        self.forcemN = int(d["forcemN"])
        self.encoderum = int(d["encoderum"])
        self.force = float(d["force"])
        self.position = float(d["position"])
        self.setpoint = int(d["setpoint"])
        self.timems = int(d["timems"])
        self.timeus = int(d["timeus"])
        self.log = int(d["log"])
        

    def getdict(self):
        return {
            "forceRaw": self.forceRaw,
            "encoderRaw": self.encoderRaw,
            "forcemN": self.forcemN,
            "encoderum": self.encoderum,
            "force": self.force,
            "position": self.position,
            "setpoint": self.setpoint,
            "timems": self.timems,
            "timeus": self.timeus,
            "log": self.log,
        }

class monitor_data_tForm(FlaskForm):
    forceRaw = IntegerField("forceRaw",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    encoderRaw = IntegerField("encoderRaw",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    forcemN = IntegerField("forcemN",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    encoderum = IntegerField("encoderum",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    force = FloatField("force",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    position = FloatField("position",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    setpoint = IntegerField("setpoint",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    timems = IntegerField("timems",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    timeus = IntegerField("timeus",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    log = IntegerField("log",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])

class MonitorDataPacket(Structure):
    _fields_ = [
        ("forcemN", c_int),
        ("encoderum", c_int),
        ("setpointum", c_int),
        ("timeus", c_int),
        ("log", c_int),
    ]

    def setdict(self, d):
        self.forcemN = int(d["forcemN"])
        self.encoderum = int(d["encoderum"])
        self.setpointum = int(d["setpointum"])
        self.timeus = int(d["timeus"])
        self.log = int(d["log"])
        

    def getdict(self):
        return {
            "forcemN": self.forcemN,
            "encoderum": self.encoderum,
            "setpointum": self.setpointum,
            "timeus": self.timeus,
            "log": self.log,
        }

class MonitorDataPacketForm(FlaskForm):
    forcemN = IntegerField("forcemN",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    encoderum = IntegerField("encoderum",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    setpointum = IntegerField("setpointum",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    timeus = IntegerField("timeus",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])
    log = IntegerField("log",validators=[validators.DataRequired(), validators.NumberRange(min=-2147483648, max=2147483647)])

