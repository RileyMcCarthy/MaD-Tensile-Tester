from app import app, socketio, ALLOWED_EXTENSIONS
from flask import render_template, request, jsonify, Response, flash, stream_with_context,send_from_directory,send_file
from werkzeug.utils import secure_filename
from .base import test_data_reciever_thread
import os
import csv
import json
@app.route('/testdata', methods=['GET'])
def data_page():
    #socketio.start_background_task(target=test_data_reciever_thread)
    files = os.listdir(app.config['TEST_FOLDER'])
    return render_template('data.html', uploaded_files=files)

@app.route('/loadtestdata', methods=['POST'])
def load_test_data():
    socketio.start_background_task(target=test_data_reciever_thread)
    return render_template('data.html')

@app.route("/download_test/<path:name>", methods=['GET','POST'])
def download_test(name):
    uploads = os.path.join(os.getcwd(), app.config['TEST_FOLDER'])
    print(uploads+"/"+name)
    return send_from_directory(
        uploads, name, as_attachment=True
    )

@app.route('/test_profile')
def stream():
    def generate():
        path = os.path.join(app.config['TEST_FOLDER'], 'test_data.csv')
        with open(path) as f:
            reader = csv.reader(f)
            test_data = {"Time": [], "Position": [], "Force": [], "Setpoint": []}
            index = 0
            for row in reader:
                test_data['Time'].append(row[0])
                test_data['Position'].append(row[1])
                test_data['Force'].append(row[2])
                test_data['Setpoint'].append(row[3])
                index += 1
                if index > 1000:
                    yield "data: %s\n\n" % json.dumps(test_data)
                    test_data = {"Time": [], "Position": [], "Force": [], "Setpoint": []}
                    index = 0
            yield "data: %s\n\n" % json.dumps(test_data)
            yield "data: done\n\n"
    return Response(stream_with_context(generate()),mimetype= 'text/event-stream')