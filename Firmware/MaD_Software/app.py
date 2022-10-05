from traceback import format_exception
from flask import Flask, render_template, request, g, session, Response, make_response, stream_with_context, redirect, url_for
from flask_session import Session
from SerialHelpers import MaD_Serial
from Helpers import *
from StateMachineDefinition import *
import random
import matplotlib
import matplotlib.pyplot as plt
import io
import time
import json
from forms import ConnectForm
from flask_wtf.csrf import CSRFProtect

# .g mutex lock, deadlock, peristance.
app = Flask(__name__)
# Check Configuration section for more details
SESSION_TYPE = 'filesystem'
app.config.from_object(__name__)
Session(app)

csrf = CSRFProtect(app)

mSerial = MaD_Serial()

app.secret_key = b'_5#y2L"F4Q8z\n\xef]/'


@app.route('/status')
def status():
    if mSerial.started == False:
        print("serial has not been started, opening connect page")
        return redirect(url_for('connect'))
    return render_template("status.html")


@app.route('/test')
def test():
    if mSerial.started == False:
        print("serial has not been started, opening connect page")
        return redirect(url_for('connect'))
    return render_template("test.html")


@app.route('/')
def home():
    # Have homepage be a first run type of page that is only displayed when no device is connected
    return redirect(url_for('status'))


@app.route('/connect', methods=['GET', 'POST'])
def connect():
    connectForm = ConnectForm()
    connectForm.port.choices = list_ports()
    print("ports:"+str(connectForm.port.choices))
    if request.method == "POST":
        if connectForm.validate():
            print("Connecting to device " + str(connectForm.port.data) +
                  " @ " + str(connectForm.baud.data))
            if mSerial.start(connectForm.port.data, connectForm.baud.data) == False:
                print("failed to connect to device")
                connectForm.port.errors.append(
                    "Failed to open specified port: " + str(connectForm.port.data))
                return render_template("connect.html", connectForm=connectForm)
            print("Initializing device")
            if mSerial.initialize() == False:
                connectForm.baud.errors.append(
                    "Failed to initialize device on " + str(mSerial.port) + " @ " + str(mSerial.baud))
                return render_template("connect.html", connectForm=connectForm)
            return redirect(url_for('status'))
        else:
            print("fields not valid")
            return render_template("connect.html", connectForm=connectForm)
    connectForm.port.default = mSerial.port
    connectForm.baud.default = mSerial.baud
    connectForm.process()
    print(connectForm.port.choices)
    print("selected port: " +
          mSerial.port + " @ " + str(mSerial.baud))
    return render_template("connect.html", connectForm=connectForm)


@app.route('/data')
def data():
    def getData():
        while True:
            if mSerial.started == False:
                print("serial has not been started")
                return ""
            print("getting data")
            data = mSerial.getMonitorData()
            if data is not None:
                jsonData = json.dumps(
                    {"time": data.timems/1000.0, "position": data.position})
                time.sleep(1)
                yield f"data:{jsonData}\n\n"
            else:
                time.sleep(1)
                yield f"data:\n\n"
    response = Response(stream_with_context(getData()),
                        mimetype="text/event-stream")
    response.headers["Cache-Control"] = "no-cache"
    response.headers["X-Accel-Buffering"] = "no"
    return response


@app.route('/ping', methods=['POST'])
def ping():
    if mSerial.started == False:
        print("serial has not been started")
        return "Disconnected"
    print("Pinging Device on port " +
          str(mSerial.port) + " @ " + str(mSerial.baud))
    if mSerial.getPing():
        return "Connected"
    else:
        return "Disconnected"


@app.route('/machineProfile', methods=['POST'])
def machineProfile():
    print("Getting machine profile")
    if mSerial.started == False:
        print("serial has not been started")
        return "Profile not available"
    deviceProfile = mSerial.getMachineProfile()
    return machine_profile_to_html(deviceProfile)


@app.route('/machineStatus', methods=['POST'])
def machineStatus():
    if request.method == "POST":
        print("Getting machineStatus")
        if mSerial.started == False:
            print("serial has not been started")
            return "Status not available"
        status = mSerial.getMachineState()
        if status is None:
            return ""
        return machine_status_to_html(status)
    return "Cannot GET /motionProfile"


@app.route('/run', methods=['POST'])
def run():
    if request.method == "POST":
        print("Running default motion profile")
        if mSerial.started == False:
            print("serial has not been started")
            return "Status not available"
        motionProfile = loadMotionProfile()
        mSerial.setMotionMode(1)
        machineState = mSerial.getMachineState()
        mSerial.setMotionProfile(motionProfile)
        mSerial.setMotionMode(2)
        machineState = mSerial.getMachineState()
    return "Cannot GET /run"


@app.route('/toggleStatus', methods=['POST'])
def toggleStatus():
    if request.method == "POST":
        print("Toggling Status")
        if mSerial.started == False:
            print("serial has not been started")
            return "Status not available"
        status = mSerial.getMotionStatus()
        print(str(status.value)+" "+str(MOTIONSTATUS_DISABLED))
        if status.value == MOTIONSTATUS_DISABLED:
            print("Enabling status")
            mSerial.setMotionStatus(MOTIONSTATUS_ENABLED)
        elif status.value == MOTIONSTATUS_ENABLED:
            print("Disabling status")
            mSerial.setMotionStatus(MOTIONSTATUS_DISABLED)
    return "Cannot GET /toggleStatus"


@app.route('/toggleMode', methods=['POST'])
def toggleMode():
    if request.method == "POST":
        print("Toggling Mode")
        if mSerial.started == False:
            print("serial has not been started")
            return "Status not available"
        mode = mSerial.getMotionMode()
        print(str(mode.value)+" "+str(MODE_MANUAL))
        if mode.value == MODE_MANUAL:
            print("Changing mode to test")
            mSerial.setMotionMode(MODE_TEST)
        elif mode.value == MODE_TEST:
            print("Changing mode to manual")
            mSerial.setMotionMode(MODE_MANUAL)
    return "Cannot GET /toggleMode"


@app.route('/motionProfile', methods=['POST'])
def motionProfile():
    if request.method == "POST":
        print("Getting motionProfile")
        if mSerial.started == False:
            print("serial has not been started")
            return "Status not available"
        profile = mSerial.getMotionProfile()
        return structure_to_html(profile)
    return "Cannot GET /motionProfile"
