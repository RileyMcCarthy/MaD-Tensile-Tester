from app import app, socketio, ALLOWED_EXTENSIONS
from flask import render_template, request, jsonify, Response, flash
from werkzeug.utils import secure_filename
from .base import gcode_sender_thread
import os

@app.route('/upload', methods=['GET'])
def upload_page():
    return render_template('upload.html')

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/upload', methods=['POST'])
def upload_file():
    # check if the post request has the file part
    if 'file' not in request.files:
        print('No file part')
        return render_template('upload.html')
    file = request.files['file']
    # If the user does not select a file, the browser submits an
    # empty file without a filename.
    if file.filename == '':
        print('No selected file')
        return render_template('upload.html')
    if not allowed_file(file.filename):
        print('File type not allowed: ' + file.filename)
        return render_template('upload.html')
    if file:
        filename = secure_filename(file.filename)
        path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(path)
        socketio.start_background_task(target=gcode_sender_thread, filename=path)
        return render_template('upload.html')
    return render_template('upload.html')