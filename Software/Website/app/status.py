from flask import render_template, Response, request
from .definitions.StateMachineDefinition import MachineState
from .helpers import ctypes_to_dict
import json
from app import app, socketio
from .helpers import flatten_dict, unflatten_dict
import app.communication as communication

#def gen_frames():
#    while True:
#        if not camera.isOpened():
#            app.logger.warning("Cannot open camera, trying again")
#            camera.open(-1)
#            continue
#        success, frame = camera.read()  # read the camera frame
#        if not success:
#            app.logger.warning("failed to read camera info")
#            break
#        else:
#            ret, buffer = cv2.imencode('.jpg', frame)
#            frame = buffer.tobytes()
#            yield (b'--frame\r\n'
#                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result
#
#
#@app.route('/video_feed')
#def video_feed():
#    app.logger.info("Starting video feed")
#    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
@app.route('/status')
def status():
    return render_template('status.html')

@app.route('/toggleMode', methods=['POST'])
def toggleMode():
    if 'mode' not in request.json:
        app.logger.error("No mode in request")
        return Response('No mode in request', status=400)
    mode = request.json['mode']
    app.logger.info("Toggling mode")
    if int(mode) == 0:
        app.logger.info("Changing mode to test")
        communication.set_motion_mode(1)
    else:
        app.logger.info("Changing mode to manual")
        communication.set_motion_mode(0)
    return Response('success')

@app.route('/toggleStatus', methods=['POST'])
def toggleStatus():
    print(request)
    if 'status' not in request.json:
        app.logger.error("No status in request")
        return Response('No status in request', status=400)
    status = request.json['status']
    app.logger.info("Toggling Status")
    if int(status) == 0:
        app.logger.info("Enabling status")
        communication.set_motion_status(1)
    else:
        app.logger.info("Disabling status")
        communication.set_motion_status(0)
    return Response('success')

@app.route('/run', methods=['POST'])
def run():
    app.logger.info("Running device motion profile")
     
    communication.set_motion_mode(2)

    return Response('success')