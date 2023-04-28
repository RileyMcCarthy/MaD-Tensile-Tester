from app import app, socketio, ALLOWED_EXTENSIONS
from flask import render_template, request, jsonify, Response, flash, redirect, url_for
from werkzeug.utils import secure_filename
from .base import gcode_sender_thread
import os
import numpy as np
from io import StringIO
import sys
import contextlib

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
        return redirect(request.url)

    file = request.files['file']
    filename = request.form['filename']

    if not filename.endswith('.gcode'):
        filename += '.gcode'
    if file and filename:
        file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        return 'File uploaded successfully.'
    else:
        return 'Error: File or filename missing.'

@app.route('/select', methods=['POST'])
def select():
    selected_file = request.form['selected_file']
    path = os.path.join(app.config['UPLOAD_FOLDER'], selected_file)
    socketio.start_background_task(target=gcode_sender_thread, filename=path)
    # Process the selected file as needed
    return f'Selected file: {selected_file}'

def run_function_string(func_str, time):
    with contextlib.redirect_stdout(StringIO()) as output:
        exec(func_str, globals())
        result = gcode_function(time)
    return result

# Add this new route after the '/select' route
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
        # Write initial G-code commands
       # f.write("G90\n")  # Absolute positioning, temporary for compatibility with old code
        #f.write("G0 X0\n")  # Move to the starting point

        # Write G0 commands for linear interpolations based on time and X positions
        prev_x = 0
        for time, x_position in zip(time_values, x_positions):
            feedrate = (x_position - prev_x) / sample_time  # Calculate feedrate in mm/s
            feedrate_mm_per_min = int(round(feedrate * 60))  # Convert feedrate to mm/minute and round to integer
            command = "G1" if feedrate_mm_per_min < 0 else "G0"
            deltax = x_position - prev_x; # temporary for compatibility with old code, should be abolsute posirtion values
            f.write(f"{command} X{deltax:.2f} F{abs(feedrate_mm_per_min)}\n")
            prev_x = x_position

    return 'file has been created!'