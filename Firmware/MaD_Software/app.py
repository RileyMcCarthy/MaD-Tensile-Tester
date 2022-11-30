from definitions.StateMachineDefinition import *
from traceback import format_exception
from flask import Flask, render_template, request, g, session, Response, make_response, stream_with_context, redirect, url_for, flash, copy_current_request_context
from SerialHelpers import MaD_Serial
from Helpers import *
import time
import json
from forms import ConnectForm, structure_to_form, MotionProfileForm, FunctionForm
from flask_wtf.csrf import CSRFProtect
import cv2
from flask_socketio import SocketIO
from threading import Lock
import csv
import socket

app = Flask(__name__)
app.config['SECRET_KEY'] = b'_5#y2L"F4Q8z\n\xef]/'
# app.config.from_object(__name__)
socketio = SocketIO(app)
csrf = CSRFProtect(app)
camera = cv2.VideoCapture(-1)
mSerial = MaD_Serial()

thread = None
thread_lock = Lock()


@app.route('/run', methods=['POST'])
def run():
    if request.method == "POST":
        app.logger.info("Running device motion profile")
        if mSerial.started == False:
            app.logger.warning("serial has not been started")
            return "Status not available"
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
    # if mSerial.started == False:
    #    app.logger.info("serial has not been started, opening connect page")
    #    return redirect(url_for('connect'))
    profile = loadMotionProfile()
    form = structure_to_form(profile)()
    form2 = MotionProfileForm(profile)()
    return render_template("test.html", form=form, form2=form2)


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
    connectForm = ConnectForm()
    connectForm.port.choices = list_ports()
    app.logger.info("ports:"+str(connectForm.port.choices))
    if request.method == "POST":
        if connectForm.validate():
            app.logger.info("Connecting to device " + str(connectForm.port.data) +
                            " @ " + str(connectForm.baud.data))
            if mSerial.start(connectForm.port.data, connectForm.baud.data) == False:
                app.logger.info("failed to connect to device")
                connectForm.port.errors.append(
                    "Failed to open specified port: " + str(connectForm.port.data))
                return render_template("connect.html", connectForm=connectForm)
            app.logger.info("Initializing device")
            if mSerial.initialize() == False:
                connectForm.baud.errors.append(
                    "Failed to initialize device on " + str(mSerial.port) + " @ " + str(mSerial.baud))
                return render_template("connect.html", connectForm=connectForm)
            return redirect(url_for('status'))
        else:
            app.logger.info("fields not valid")
            return render_template("connect.html", connectForm=connectForm)
    connectForm.port.default = mSerial.port
    connectForm.baud.default = mSerial.baud
    connectForm.process()
    app.logger.info(connectForm.port.choices)
    app.logger.info("selected port: " +
                    mSerial.port + " @ " + str(mSerial.baud))
    return render_template("connect.html", connectForm=connectForm)


@app.route('/flashData')
def flashData():
    with open("data.csv", "r") as fp:
        csv = fp.read()
    return Response(
        csv,
        mimetype="text/csv",
        headers={"Content-disposition":
                 "attachment; filename=myplot.csv"})
    return output

    def getData():
        try:
            with open("data.csv", "r") as file:
                # time,position,force,setpoint
                app.logger.info(file.readline())
                csvFile = csv.reader(file, delimiter=',')
                for data in csvFile:
                    jsonData = json.dumps({"time": float(data[0]), "position": float(data[1]),
                                           "force": float(data[2]), "setpoint": float(data[3])})
                    app.logger.info(jsonData)
                    yield f"data:{jsonData}\n\n"
        finally:
            yield f""
            app.logger.info("Completed flash data stream")
    response = Response(getData(),
                        mimetype="text/event-stream")
    response.headers["Cache-Control"] = "no-cache"
    response.headers["X-Accel-Buffering"] = "no"
    return response


@app.route('/functionManual', methods=['GET', 'POST'])
def functionManual():
    # this is crashing serial, nothing happens currently
    return ""
    if request.method == "POST":
        if mSerial.started == False:
            app.logger.info("serial has not been started")
        else:
            app.logger.info("Running function " + request.data.function)
            mSerial.setMotionFunction(request.data.function, 100)
    app.logger.info("Getting current function and data")
    info = mSerial.getMotionFunction()
    if info is None:
        return ""
    func, data = info
    app.logger.info(func)
    app.logger.info(data)
    return FunctionForm(func, data)


@app.route('/ping', methods=['POST'])
def ping():
    if mSerial.started == False:
        app.logger.info("serial has not been started")
        return "Disconnected"
    app.logger.info("Pinging Device on port " +
                    str(mSerial.port) + " @ " + str(mSerial.baud))
    if mSerial.getPing():
        return "Connected"
    else:
        return "Disconnected"


@app.route('/machineProfile', methods=['POST'])
def machineProfile():
    app.logger.info("Getting machine profile")
    if mSerial.started == False:
        app.logger.info("serial has not been started")
        return "Profile not available"
    deviceProfile = mSerial.getMachineProfile()
    return machine_profile_to_html(deviceProfile)


@app.route('/machineStatusStream')
def machineStatusStream():
    def getStatusStream():
        try:
            while True:
                status = mSerial.getMachineState()
                if status is not None:
                    html = machine_status_to_html(status)
                    app.logger.info("sending status")
                    yield json.dumps({"Status": html})
                time.sleep(1)
        finally:
            app.logger.info("status stream closed")
    response = Response(stream_with_context(getStatusStream()),
                        mimetype="text/event-stream")
    # response.headers["Cache-Control"] = "no-cache"
    # response.headers["X-Accel-Buffering"] = "no"
    return response


@app.route('/machineStatus', methods=['POST'])
def machineStatus():
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
        # if mSerial.started == False:
        # app.logger.info("serial has not been started")
        #   return "Status not available"
        # profile = mSerial.getMotionProfile()
        profile = loadMotionProfile()
        return structure_to_html(profile)
    return "Cannot GET /motionProfile"
