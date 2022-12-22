from flask import render_template, request
from .definitions.JSON import MachineProfile
from .helpers import ctypes_to_dict,dict_to_ctypes, loadMachineProfile

from app import app, socketio
import app.communication as communication

@app.route('/settings', methods=['GET', 'POST'])
def settings():
    my_dict = ctypes_to_dict(loadMachineProfile())
    if request.method == 'POST':
        for key, value in request.form.items():
            parts = key.split('.')
            d = my_dict
            for p in parts[:-1]:
                if p in d:
                    d = d[p]
            d[parts[-1]] = value
    communication.set_machine_profile(dict_to_ctypes(my_dict, MachineProfile))
    print(my_dict)
    return render_template('settings.html', machineProfile=my_dict)
