from ctypes import Structure, c_int, c_float, c_char, c_bool
from wtforms import BooleanField,IntegerField, StringField,FloatField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
CMD_WRITE = 128
CMD_PING = 0           # test communication
CMD_DATA = 1           # send monitor data
CMD_STATE = 2          # send machine state
CMD_MPROFILE = 3       # send/recieve machine profile
CMD_MCONFIG = 4        # send/recieve machine configuration
CMD_MPERFORMANCE = 5   # send/receive machine performance
CMD_MOTIONPROFILE = 6  # send/recieve motion profile
CMD_MOTIONMODE = 7     # send/recieve motion mode
CMD_MOTIONFUNCTION = 8 # send/recieve motion function and data
CMD_MOTIONSTATUS = 9   # send/recieve motion status
MAD_VERSION = 1
