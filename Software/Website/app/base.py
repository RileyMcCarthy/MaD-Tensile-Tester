from flask import render_template
from queue import Queue
from .helpers import ctypes_to_dict
from app.definitions import MonitorDataPacket, MachineState, MachineProfile, MachineConfiguration, MachinePerformance, MotionProfile, MotionMode, MotionStatus
import json

from app import app, socketio
import app.communication as communication

def serial_thread():
    print('starting serial task')
    while not communication.start():
        print('serial failed to start')
        socketio.sleep(1)
    while True:
        data = communication.process_recieved()
        if data is not None:
            #print(f'got data: {data}')
            if type(data) == MachineState:
                #print(ctypes_to_dict(data))
                socketio.emit('state', {'json': json.dumps(ctypes_to_dict(data))}, namespace = '/serial')
            elif type(data) == MonitorDataPacket:
                socketio.emit('data', {'json': json.dumps(ctypes_to_dict(data))}, namespace = '/serial')
        socketio.sleep(0.1)

def data_thread():
    print('starting data task')
    while True:
        #print('Sending data command')
        communication.get_data()
        socketio.sleep(0.5)

def state_thread():
    print('starting status task')
    number = 0
    while True:
        #print(f'Sending status command: {number}')
        number += 1
        communication.get_state()
        socketio.sleep(1)

@app.route('/index')
def index():
    return render_template('base.html')

@socketio.on('connect', namespace='/serial')
def serial_client_connect():
    pass