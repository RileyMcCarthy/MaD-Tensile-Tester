from flask import render_template, Response, request
from .helpers import ctypes_to_dict
import json
from app import app, socketio
from .helpers import flatten_dict, unflatten_dict
import app.communication as communication
import cv2
import os
from .base import gcode_sender_thread, gcode_to_dict, emit_notification, generate_gcode_from_profile, set_gcode_sender_thread_path

camera = cv2.VideoCapture(-1)

last_serial_test_run_number = {}

def gen_frames():
    while True:
        success, frame = camera.read()  # read the camera frame
        if not success:
            app.logger.warning("failed to read camera info")
            break
        else:
            ret, buffer = cv2.imencode('.jpg', frame)
            frame = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result

@app.route('/video_feed')
def video_feed():
    app.logger.info("Starting video feed")
    if not camera.isOpened():
            app.logger.warning("Cannot open camera, trying again")
            socketio.sleep(5)
            camera.open(-1)
            return Response(b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + b'\r\n', mimetype='multipart/x-mixed-replace; boundary=frame')
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
@app.route('/status')
def status():
    profile_files = os.listdir(app.config['PROFILE_FOLDER'])
    print(profile_files)
    return render_template('status.html', motion_profiles = profile_files)

# create endpoint "/status/run_number/" + serial_number, which returns the last run number for that serial number
@app.route('/status/run_number/<serial_number>')
def get_last_run_number(serial_number):
    if serial_number in last_serial_test_run_number:
        return Response(str(last_serial_test_run_number[serial_number]))
    else:
        return Response('0')

@app.route('/status/setup_test', methods=['POST'])
def setup_test():
    test_description = request.form.get('test_description')
    test_name = request.form.get('test_name')
    run_number = request.form.get('run_number')
    data_time = request.form.get('data_time')
    serial_number = request.form.get('serial_number')
    motion_profile = request.form.get('motion_profile')
    gauge_length = request.form.get('gauge_length')

    header = {
        'test_description': test_description,
        'test_name': test_name,
        'run_number': run_number,
        'date_time': data_time,
        'sample_serial_number': serial_number,
        'motion_profile': motion_profile,
        'gauge_length': gauge_length
    }

    global last_serial_test_run_number
    last_serial_test_run_number[serial_number] = run_number

    communication.set_test_header(header)
    path = os.path.join(app.config['PROFILE_FOLDER'], motion_profile)
    # TODO add method of killing thread if it is already running
    set_gcode_sender_thread_path(path)
    return Response('success')


@app.route('/toggleMode', methods=['POST'])
def toggleMode():
    if 'mode' not in request.json:
        app.logger.error("No mode in request")
        return Response('No mode in request', status=400)
    mode = request.json['mode']
    app.logger.info("Toggling mode")
    if mode == "MANUAL":
        app.logger.info("Changing mode to test")
        communication.set_motion_mode("TEST")
    else:
        app.logger.info("Changing mode to manual")
        communication.set_motion_mode("MANUAL")
    return Response('success')

@app.route('/toggleStatus', methods=['POST'])
def toggleStatus():
    print(request)
    if 'status' not in request.json:
        app.logger.error("No status in request")
        return Response('No status in request', status=400)
    status = request.json['status']
    app.logger.info("Toggling Status")
    if status == "DISABLED":
        app.logger.info("Enabling status")
        communication.set_motion_status("ENABLED")
    else:
        app.logger.info("Disabling status")
        communication.set_motion_status("DISABLED")
    return Response('success')

@app.route('/set_gauge_length', methods=['POST'])
def gauge_length():
    print("setting gauge length")
    communication.set_gauge_length()
    return Response('success')

@app.route('/set_gauge_force', methods=['POST'])
def gauge_force():
    print("setting gauge force")
    communication.set_gauge_force()
    return Response('success')

@app.route('/run', methods=['POST'])
def run():
    app.logger.info("Running device motion profile")
    communication.run_test()
    return Response('success')