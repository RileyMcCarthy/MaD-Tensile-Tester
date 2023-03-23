from flask import render_template, request, jsonify, Response
from .definitions.JSON import MachineProfile, MachinePerformance, MachineProfileForm
from .helpers import ctypes_to_dict,dict_to_ctypes, loadMachineProfile
from werkzeug.datastructures import MultiDict
from app import app, socketio
import app.communication as communication
from .helpers import flatten_dict, unflatten_dict

@app.route('/settings', methods=['GET'])
def settings():
    form = MachineProfileForm()
    return render_template('settings.html',form=form, editable=False)

@app.route('/settings', methods=['POST'])
def settings_save():
    form = MachineProfileForm()
    if form.validate_on_submit():
        result = unflatten_dict(request.form.to_dict())
        mp = MachineProfile()
        mp.setdict(result)
        communication.set_machine_profile(mp)
    else:
        print(form.errors)
        return Response(jsonify(flatten_dict(form.errors)), status=400)
    return Response('success')

@app.route('/settings/edit', methods=['GET'])
def settings_edit():
    form = MachineProfileForm()
    return render_template('settings.html', form=form, editable=True)

@app.route('/settings/defaults', methods=['GET', 'POST'])
def settings_default():
    mp = loadMachineProfile().getdict()
    form = MachineProfileForm()
    form.process(data=mp)
    return render_template('settings.html', form=form, editable=True)
