from wtforms import Form, BooleanField, StringField, PasswordField, validators, SelectField, SubmitField
from flask_wtf import FlaskForm


class ConnectForm(FlaskForm):
    port = SelectField('Serial Port', choices=[''], validate_choice=False)
    baud = SelectField('Baud Rate', choices=[
                       9600, 14400, 19200, 38400, 57600, 115200, 230400, 256000], validate_choice=False)
