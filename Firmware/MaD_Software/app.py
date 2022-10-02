from traceback import format_exception
from flask import Flask, render_template, request, g, session, Response, make_response, stream_with_context
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
# .g mutex lock, deadlock, peristance.
app = Flask(__name__)
# Check Configuration section for more details
SESSION_TYPE = 'filesystem'
app.config.from_object(__name__)
Session(app)

mSerial = MaD_Serial()

app.secret_key = b'_5#y2L"F4Q8z\n\xef]/'


@app.route('/formTest', methods=['GET', 'POST'])
def register():
    form = ConnectForm(request.form)
    if request.method == 'POST' and form.validate():
        print("Form submitted! Port:"+form.port+", Baud:"+form.baud)
    return ""

@app.route('/status')
def status():
    return render_template("status.html")


@app.route('/test')
def test():
    return render_template("test.html")


@app.route('/')
def home():
    ports = list_ports()
    connectForm = ConnectForm()
    print(ports)
    print("selected port: " +
          mSerial.port + " @ " + str(mSerial.baud))
    return render_template("home.html", ports=ports, selectedPort=mSerial.port, baud=mSerial.baud, connectForm=connectForm)


@app.route('/connect', methods=['POST'])
def connect():
    print("Getting data")
    port = request.form['port']
    baud = request.form['baud']
    if mSerial.start(port, baud) == False:
        return "Failed to open specified port: " + str(port) + " @ " + str(baud)
    if mSerial.initialize() == False:
        return "Failed to initialize device on " + str(mSerial.port) + " @ " + str(mSerial.baud)
    return "Connected"
    #machineState = getMachineState(ser)
    # print_ctypes_obj(machineState)
    #monitorData = getMonitorData(ser)
    # print_ctypes_obj(monitorData)
    #motionProfile = loadMotionProfile()
    #setMotionMode(ser, 1)
    #machineState = getMachineState(ser)
    # print_ctypes_obj(machineState)
    #setMotionProfile(ser, motionProfile)
    #setMotionMode(ser, 2)
    #machineState = getMachineState(ser)
    # time.sleep(15)
    # print_ctypes_obj(machineState)

    dataList = getTestData(ser)
    timeY = []
    force = []
    position = []
    startTime = dataList[0].timeus
    for data in dataList:
        if (data.timeus > 0):
            timeY.append((data.timeus-startTime)/1000000.0)
            force.append(data.force)
            position.append(data.position)
    print(timeY)
    plt.plot(timeY, force)
    plt.xlabel("Time (s)")
    plt.ylabel("Force (N)")
    plt.savefig('static/images/force.png')
    plt.clf()

    plt.plot(timeY, position)
    plt.xlabel("Time (ms)")
    plt.ylabel("position (N)")
    plt.savefig('static/images/position.png')
    plt.clf()

    ser.close()
    if (connected == True):
        return "Connected"
    else:
        return "Disconnected"
    machineState = getMachineState(ser)
    print_ctypes_obj(machineState)
    monitorData = getMonitorData(ser)
    print_ctypes_obj(monitorData)
    motionProfile = loadMotionProfile()
    setMotionMode(ser, 1)
    machineState = getMachineState(ser)
    print_ctypes_obj(machineState)
    setMotionProfile(ser, motionProfile)
    setMotionMode(ser, 2)
    machineState = getMachineState(ser)
    time.sleep(15)
    print_ctypes_obj(machineState)
    return "Connected"
    dataList = getTestData(ser)
    x = []
    y = []
    for data in dataList:
        x = data.timeus
        y = data.force
    session['x'] = x
    session['y'] = y
    print(dataList)
    if (connected == True):
        return "Connected"
    else:
        return "Disconnected"


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
