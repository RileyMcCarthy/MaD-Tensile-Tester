from flask import render_template, request, jsonify, Response
from .helpers import loadMachineProfile
from werkzeug.datastructures import MultiDict
from app import app, socketio
import app.communication as communication
from .helpers import flatten_dict, unflatten_dict
import json

@app.route('/settings', methods=['GET'])
def settings():
    return render_template('settings.html')

@app.route('/settings', methods=['POST'])
def settings_save():
    if (request.json is None):
        return Response('No form data', status=400)

    # Convert form data to dict
    print(request.json)
    communication.set_machine_profile(request.json)
    
    return Response('success')

#@app.route('/settings/edit', methods=['GET'])
#def settings_edit():
#    form = MachineProfileForm()
#    return render_template('settings.html', form=form, editable=True)
#
#@app.route('/settings/defaults', methods=['GET', 'POST'])
#def settings_default():
#    mp = loadMachineProfile().getdict()
#    form = MachineProfileForm()
#    form.process(data=mp)
#    return render_template('settings.html', form=form, editable=True)
