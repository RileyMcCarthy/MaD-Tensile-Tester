from app import app, socketio, ALLOWED_EXTENSIONS
from flask import render_template, request, jsonify, Response, flash, redirect, url_for
from werkzeug.utils import secure_filename
from .base import gcode_sender_thread, gcode_to_dict
import app.communication as communication
import os
import numpy as np
from io import StringIO
import sys
import contextlib
import json

def run_function_string(func_str, time):
    with contextlib.redirect_stdout(StringIO()) as output:
        exec(func_str, globals())
        result = gcode_function(time)
    return result

@app.template_filter('with_index')
def with_index(iterable):
    return zip(range(len(iterable)), iterable)

profile = {
    "name": "",
    "sets": []
}

@app.route('/create/')
def index():
    print(profile)
    files = os.listdir(app.config['PROFILE_FOLDER'])
    return render_template('create.html', profile=profile, files=files)

@app.route('/create/load', methods=['POST'])
def load_profile():
    selected_file = request.form['selected_file']
    print(selected_file)
    profile_filepath = os.path.join(app.config['PROFILE_FOLDER'], selected_file)
    print(profile_filepath)
    with open(profile_filepath) as f:
        global profile
        profile = json.load(f)
        print(profile)
    return redirect(url_for('index'))

@app.route('/create/save', methods=['POST'])
def save_profile():
    # Write profile json to file
    profile_name = profile['name']
    if not profile_name.endswith('.json'):
        profile_name += '.json'
    profile_filepath = os.path.join(app.config['PROFILE_FOLDER'], profile_name)
    with open(profile_filepath, 'w') as f:
        json.dump(profile, f)
    print("Saved profile:", profile)
    return jsonify({'message': 'Profile saved successfully'})

@app.route('/create/add_set', methods=['POST'])
def add_set():
    new_set = {
        "name": "",
        "executions": 1,
        "gcode": "G1 X0.0 F100"
    }
    profile["sets"].append(new_set)
    return jsonify({'message': 'Set added successfully'})

@app.route('/create/remove_set/<int:set_index>', methods=['POST'])
def remove_set(set_index):
    profile["sets"].pop(set_index)
    return jsonify({'message': 'Set removed successfully'})

@app.route('/create/update_profile', methods=['POST'])
def update_profile():
    data = request.json
    profile['name'] = data.get('profile_name', profile['name'])
    return jsonify({'message': 'Profile name updated successfully'})

@app.route('/create/update_set/<int:set_index>', methods=['POST'])
def update_set(set_index):
    set_obj = profile["sets"][set_index]
    data = request.json
    set_obj['name'] = data.get('set_name', set_obj['name'])
    set_obj['executions'] = int(data.get('set_executions', set_obj['executions']))
    set_obj['gcode'] = (data.get('set_gcode', set_obj['gcode']))
    print(set_obj['executions'])
    return jsonify({'message': 'Set name updated successfully'})

@app.route('/generate', methods=['POST'])
def generate_gcode():
    function_str = request.form['function']
    sample_time = float(request.form['sample_time'])
    total_time = float(request.form['total_time'])

    func = lambda x: run_function_string(function_str, x)

    # Generate G-code file
    gcode_filename = request.form['gcode_filename']
    if not gcode_filename.endswith('.gcode'):
        gcode_filename += '.gcode'
    gcode_filepath = os.path.join(app.config['UPLOAD_FOLDER'], gcode_filename)

    # Calculate time values and x_positions (results of func(time))
    time_values = [i * sample_time for i in range(int(total_time / sample_time) + 1)]
    x_positions = [func(time) for time in time_values]
    
    with open(gcode_filepath, 'w') as f:
        prev_x = 0
        for x_position in x_positions:
            feedrate = 60*(x_position - prev_x) / sample_time  # Calculate feedrate in mm/min
            feedrate_mm_per_min = int(round(feedrate))  # Convert feedrate to um/s and round to integer
            command = "G1"
            f.write(f"{command} X{x_position} F{feedrate_mm_per_min}\n")
            prev_x = x_position
        f.write(f"G122") # Indicate end of program
    return 'file has been created!'
