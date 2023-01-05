from ctypes import Structure, c_int, c_float, c_char, c_bool
from wtforms import BooleanField,IntegerField, StringField,FloatField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
FUNCTION_COUNT = 2
QUARTET_FUNC_LINE = 0
QUARTET_FUNC_SIGMOIDAL = 1
