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

#@app.route('/create', methods=['GET'])
#def create_page():
#    files = os.listdir(app.config['UPLOAD_FOLDER'])
#    return render_template('create.html', files=files)

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
