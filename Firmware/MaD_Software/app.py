from definitions.StateMachineDefinition import *
from traceback import format_exception
from flask import Flask, render_template, request, g, session, Response, make_response, stream_with_context, redirect, url_for, flash, copy_current_request_context
from SerialHelpers import MaD_Serial
from Helpers import *
import time
import json
from forms import structure_to_form, MotionProfileForm, FunctionForm
from flask_wtf.csrf import CSRFProtect
from flask_wtf import FlaskForm
import cv2
from flask_socketio import SocketIO
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config['SECRET_KEY'] = b'_5#y2L"F4Q8z\n\xef]/'
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///project.db"

db = SQLAlchemy(app)

socketio = SocketIO(app)
csrf = CSRFProtect(app)
camera = cv2.VideoCapture(-1)

@app.route('/run', methods=['POST'])
def run():
    if request.method == "POST":
        app.logger.info("Running device motion profile")
        # TODO: Remove this for loading from file
        motionProfile = loadMotionProfile()
        mSerial.setMotionProfile(motionProfile)
        mSerial.setMotionMode(2)
        mode = mSerial.getMotionMode()
        if mode != 2:
            app.logger.info("unable to run test. Mode: "+str(mode))
            return "Unable to run test"
    return "Cannot GET /run"


def gen_frames():
    while True:
        if not camera.isOpened():
            app.logger.warning("Cannot open camera, trying again")
            camera.open(-1)
            continue
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
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')


@app.route('/status')
def status():
    if mSerial.started == False:
        app.logger.info("serial has not been started, opening connect page")
        return redirect(url_for('connect'))
    return render_template("status.html")


@app.route('/test')
def test():
    profile = loadMotionProfile()
    form = structure_to_form(profile)()
    form2 = MotionProfileForm(profile)()
    return render_template("test.html", form=form, form2=form2)


@app.route('/settings')
def settings():
    return render_template("settings.html")


@app.route('/dataPage')
def dataPage():
    if mSerial.started == False:
        app.logger.info("serial has not been started, opening connect page")
        return redirect(url_for('connect'))
    return render_template("data.html")


@app.route('/')
def home():
    # Have homepage be a first run type of page that is only displayed when no device is connected
    return redirect(url_for('status'))


@app.route('/connect', methods=['GET', 'POST'])
def connect():
    return render_template("connect.html")


@app.route('/machineProfile', methods=['POST'])
def machineProfile():
    app.logger.info("Getting machine profile")
    if mSerial.started == False:
        app.logger.info("serial has not been started")
        return "Profile not available"
    deviceProfile = mSerial.getMachineProfile()
    return machine_profile_to_html(deviceProfile)


@app.route('/machineStatus', methods=['POST'])
def machineStatus():
    # @TODO should trigger a status update using status thread
    if request.method == "POST":
        app.logger.info("Getting machineStatus")
        if mSerial.started == False:
            app.logger.info("serial has not been started")
            return "Status not available"
        status = mSerial.getMachineState()
        if status is None:
            return ""
        return machine_status_to_html(status)
    return "Cannot GET /machineStatus"


@app.route('/toggleStatus', methods=['POST'])
def toggleStatus():
    if request.method == "POST":
        app.logger.info("Toggling Status")
        if mSerial.started == False:
            app.logger.info("serial has not been started")
            return "Status not available"
        status = mSerial.getMotionStatus()
        if status == MOTIONSTATUS_DISABLED:
            app.logger.info("Enabling status")
            mSerial.setMotionStatus(MOTIONSTATUS_ENABLED)
        elif status == MOTIONSTATUS_ENABLED:
            app.logger.info("Disabling status")
            mSerial.setMotionStatus(MOTIONSTATUS_DISABLED)
    return "Cannot GET /toggleStatus"


@app.route('/toggleMode', methods=['POST'])
def toggleMode():
    if request.method == "POST":
        app.logger.info("Toggling Mode")
        if mSerial.started == False:
            app.logger.info("serial has not been started")
            return "Status not available"
        mode = mSerial.getMotionMode()
        if mode == MODE_MANUAL:
            app.logger.info("Changing mode to test")
            mSerial.setMotionMode(MODE_TEST)
        elif mode == MODE_TEST:
            app.logger.info("Changing mode to manual")
            mSerial.setMotionMode(MODE_MANUAL)
    return "Cannot GET /toggleMode"


@app.route('/motionProfile', methods=['POST'])
def motionProfile():
    if request.method == "POST":
        app.logger.info("Getting motionProfile")
        profile = loadMotionProfile()
        return structure_to_html(profile)
    return "Cannot GET /motionProfile"
