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
    return render_template('create.html', profile=profile)

@app.route('/create/add_set', methods=['POST'])
def add_set():
    new_set = {
        "name": "",
        "quartets": []
    }
    profile["sets"].append(new_set)
    return jsonify({'message': 'Set added successfully'})

@app.route('/create/remove_set/<int:set_index>', methods=['POST'])
def remove_set(set_index):
    profile["sets"].pop(set_index)
    return jsonify({'message': 'Set removed successfully'})

@app.route('/create/add_quartet/<int:set_index>', methods=['POST'])
def add_quartet(set_index):
    set_obj = profile["sets"][set_index]
    data = request.json
    new_quartet = {
        "name": data.get('quartet_name', ''),
        "function": data.get('quartet_function', None),
        "execution_time": float(data.get('quartet_execution_time', 0.0)),
        "dwell": float(data.get('quartet_dwell', 0.0))
    }
    set_obj["quartets"].append(new_quartet)
    return jsonify({'message': 'Quartet added successfully'})

@app.route('/create/remove_quartet/<int:set_index>/<int:quartet_index>', methods=['POST'])
def remove_quartet(set_index, quartet_index):
    set_obj = profile["sets"][set_index]
    set_obj["quartets"].pop(quartet_index)
    return jsonify({'message': 'Quartet removed successfully'})

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
    return jsonify({'message': 'Set name updated successfully'})

@app.route('/create/update_quartet/<int:set_index>/<int:quartet_index>', methods=['POST'])
def update_quartet(set_index, quartet_index):
    set_obj = profile["sets"][set_index]
    quartet = set_obj["quartets"][quartet_index]
    data = request.json
    quartet['name'] = data.get('quartet_name', '')
    quartet['function'] = data.get('quartet_function', None)
    quartet['execution_time'] = float(data.get('quartet_execution_time', 0.0))
    quartet['dwell'] = float(data.get('quartet_dwell', 0.0))
    return jsonify({'message': 'Quartet updated successfully'})
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
