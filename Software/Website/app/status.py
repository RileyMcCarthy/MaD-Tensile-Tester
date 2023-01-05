from flask import render_template, Response
from .definitions.StateMachineDefinition import MachineState
from .helpers import ctypes_to_dict
import json
from app import app, socketio
from .helpers import flatten_dict, unflatten_dict

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
    return render_template('status.html', state=flatten_dict(MachineState().getdict()))