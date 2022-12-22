from flask import render_template, Response
from .definitions.JSON import MachineProfile
from .helpers import ctypes_to_dict

from app import app, socketio

@app.route('/settings')
def settings():
    return render_template('settings.html', machineProfile=ctypes_to_dict(MachineProfile()))