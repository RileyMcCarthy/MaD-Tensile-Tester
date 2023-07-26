from app import app, socketio, ALLOWED_EXTENSIONS
from flask import render_template, request, jsonify, Response, flash, redirect, url_for
from werkzeug.utils import secure_filename
from .base import gcode_sender_thread, gcode_to_dict, emit_notification
import app.communication as communication
import os
import numpy as np
from io import StringIO
import sys
import contextlib
import json

@app.route('/upload', methods=['GET'])
def upload_page():
    files = os.listdir(app.config['UPLOAD_FOLDER'])
    return render_template('upload.html', files=files)

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        emit_notification('error', 'No file found in request!')
        return redirect(url_for('upload_page'))

    file = request.files['file']
    filename = request.form['filename']

    if not filename.endswith('.gcode'):
        filename += '.gcode'
    if file and filename:
        file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        emit_notification('success', 'File uploaded successfully!')
    else:
        emit_notification('error', 'No file selected!')
    return redirect(url_for('upload_page'))

@app.route('/select', methods=['POST'])
def select():
    selected_file = request.form['selected_file']
    path = os.path.join(app.config['UPLOAD_FOLDER'], selected_file)
    socketio.start_background_task(target=gcode_sender_thread, filename=path)
    return f"Sending {selected_file} file to machine"

@app.route('/gcode_file_to_moves', methods=['POST'])
def gcode_file_to_moves():
    selected_file_json = request.get_json()
    selected_file = selected_file_json['filename']
    print(selected_file)
    path = os.path.join(app.config['UPLOAD_FOLDER'], selected_file)
    with open(path) as f:
        lines = f.readlines()
    # Convert gcode lines to array of dict
    cordinates = []
    last_position = 0
    time = 0
    for line in lines:
        print(f'sending gcode: {line}')
        command = gcode_to_dict(line)
        if command['G'] == 4:
            position = last_position
            time += command['P']/1000.0
        elif command['G'] == 0 or command['G'] == 1:
            position = command['X'] # mm
            feedrate = command['F'] # mm/min
            if feedrate != 0:
                time += (position-last_position)/(feedrate/60)
        elif command['G'] == 122:
            break # end of gcode file
        else:
            continue # Skip unknown gcode commands
        cord = {"X": time, "Y": position}
        cordinates.append(cord)
        last_position = position
    return json.dumps(cordinates)

@app.route('/jog', methods=['POST'])
def jog_machine():
    g = int(request.form['g'])
    x = int(request.form['x'])
    f = int(request.form['f'])
    command = {'G': g, 'X': x, 'F': f}
    print(command)
    communication.set_manual_command(command)
    return redirect(url_for('upload_page'))

@app.route('/set_gauge_length', methods=['POST'])
def gauge_length():
    print("setting gauge length")
    communication.set_gauge_length()
    return redirect(url_for('upload_page'))

class MotionQuartet:
    def __init__(self):
        self.name = ""
        self.function = None # Format: position = function(time)
        self.execution_time = 0.0
        self.dwell = 0.0

class MotionSet:
    def __init__(self):
        self.name = ""
        self.executions = 0
        self.quartets = []

class MotionProfile:
    def __init__(self):
        self.name = ""
        self.sets = []

profile = MotionProfile()
set1 = MotionSet()
quartet1 = MotionQuartet()

set1.quartets.append(quartet1)
profile.sets.append(set1)

# Create a sample motion profile
profile = MotionProfile()
set1 = MotionSet()
quartet1 = MotionQuartet()
set1.quartets.append(quartet1)
profile.sets.append(set1)

# Define routes
@app.route('/create')
def index():
    return render_template('create.html', profile=profile)

@app.route('/create/add_set', methods=['POST'])
def add_set():
    name = request.form['set_name']
    new_set = MotionSet()
    new_set.name = name
    profile.sets.append(new_set)
    return redirect('/create')

@app.route('/create/remove_set/<int:set_index>', methods=['POST'])
def remove_set(set_index):
    profile.sets.pop(set_index)
    return redirect('/create')

@app.route('/create/add_quartet/<int:set_index>', methods=['POST'])
def add_quartet(set_index):
    set_obj = profile.sets[set_index]
    new_quartet = MotionQuartet()
    set_obj.quartets.append(new_quartet)
    return redirect('/create')

@app.route('/create/remove_quartet/<int:set_index>/<int:quartet_index>', methods=['POST'])
def remove_quartet(set_index, quartet_index):
    set_obj = profile.sets[set_index]
    set_obj.quartets.pop(quartet_index)
    return redirect('/create')