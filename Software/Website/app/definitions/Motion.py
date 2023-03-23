from ctypes import Structure, c_int, c_float, c_char, c_bool
from wtforms import BooleanField,IntegerField, StringField,FloatField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
class MotionCommand(Structure):
    _fields_ = [
        ("steps", c_int),
        ("feedrate", c_int),
        ("last", c_bool),
    ]

    def setdict(self, d):
        self.steps = int(d["steps"])
        self.feedrate = int(d["feedrate"])
        self.last = bool(d["last"])
        

    def getdict(self):
        return {
            "steps": self.steps,
            "feedrate": self.feedrate,
            "last": self.last,
        }

class MotionCommandForm(FlaskForm):
    steps = IntegerField("steps",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    feedrate = IntegerField("feedrate",validators=[ validators.NumberRange(min=-2147483648, max=2147483647)])
    last = BooleanField("last",validators=[validators.DataRequired()])

