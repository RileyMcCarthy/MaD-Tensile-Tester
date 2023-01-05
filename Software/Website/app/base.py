from flask import render_template
from queue import Queue
from .helpers import ctypes_to_dict
from app.definitions import MonitorDataPacket, MachineState, MachineProfile, MachineConfiguration, MachinePerformance, MotionProfile, MotionMode, MotionStatus
import json
from app import app, socketio
import app.communication as communication
from collections.abc import MutableMapping
from .helpers import flatten_dict

def serial_thread():
    # Serial thread is responsible for reading the serial port and sending the data to the client
    print('starting serial task')
    while not communication.start():
        print('serial failed to start')
        socketio.sleep(1)
    while True:
        data = communication.process_recieved()
        if data is not None:
            data_json = json.dumps(flatten_dict(data.getdict()))
            #print(f'got data: {data_json}')
            if type(data) == MachineState:
                socketio.emit('state', {'json': data_json}, namespace = '/serial')
            elif type(data) == MonitorDataPacket:
                socketio.emit('data', {'json': data_json}, namespace = '/serial')
            elif type(data) == MachineProfile:
                #print(data_json)
                socketio.emit('profile', {'json': data_json}, namespace = '/serial')
        socketio.sleep(0.1)

def data_thread():
    # Data thread is respondsible for sending the data request command to the serial port
    print('starting data task')
    while True:
        #print('Sending data command')
        communication.get_data()
        socketio.sleep(0.5)

def state_thread():
    # State thread is respondsible for sending the state request command to the serial port
    print('starting status task')
    number = 0
    while True:
        #print(f'Sending status command: {number}')
        number += 1
        communication.get_state()
        socketio.sleep(1)

@socketio.on('profile', namespace='/serial')
def serial_client_profile():
    communication.get_machine_profile()

@socketio.on('connect', namespace='/serial')
def serial_client_connect():
    pass