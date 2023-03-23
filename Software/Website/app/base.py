from flask import render_template, request
from queue import Queue
from .helpers import ctypes_to_dict
from app.definitions.Motion import MotionCommand
from app.definitions.Monitor import MonitorDataPacket, MonitorData
from app.definitions.StateMachineDefinition import MachineState, MotionMode, MotionStatus
from app.definitions.JSON import MachineProfile, MachineConfiguration, MachinePerformance
import json
from app import app, socketio
import app.communication as communication
from app.communication.commands import CMD_AWK, CMD_TESTDATA,CMD_TESTDATA_COUNT
from collections.abc import MutableMapping
from .helpers import flatten_dict
from threading import Lock
import re
import time
import csv
from datetime import datetime


gcode_lock = Lock()
gcode_ack = "NONE"

test_data_lock = Lock()
test_data = None
test_data_count = 0
test_data_complete = False

def serial_thread():
    # Serial thread is responsible for reading the serial port and sending the data to the client
    print('starting serial task')
    while not communication.start():
        print('serial failed to start')
        socketio.sleep(1)
    while True:
        res = communication.process_recieved()
        if res is not None:
            cmd, data = res
            #print(f'got data: {data_json}')
            if type(data) == MachineState:
                data_json = data.getdict()
                socketio.emit('state', {'json': json.dumps(data_json)}, namespace = '/serial')
            elif type(data) == MonitorDataPacket:  
                data_json = data.getdict()
                socketio.emit('data', {'json': json.dumps(data_json)}, namespace = '/serial')
            elif type(data) == MachineProfile:
                data_json = data.getdict()
                #print(data_json)f
                socketio.emit('profile', {'json': json.dumps(flatten_dict(data_json))}, namespace = '/serial')
            elif cmd == CMD_AWK:
                data_integer = int.from_bytes(data, byteorder='big')
                print('Recieving ack command: ', data_integer)
                global gcode_ack
                if data_integer == 0:
                    gcode_ack = "OK"
                    print('got ack')
                elif data_integer == 1:
                    gcode_ack = "FAIL"
                    print('got nak')
                elif data_integer == 2:
                    gcode_ack = "BUSY"
                    print('busy')
            elif cmd == CMD_TESTDATA:
                print('Recieving test data: ', data)
                global test_data_complete
                global test_data
                test_data_complete = data is None
                test_data = data
                print('got:',test_data,' of size ',len(test_data))
                #socketio.emit('test_data', {'json': json.dumps(data_integer)}, namespace = '/serial')
            elif cmd == CMD_TESTDATA_COUNT:
                #print('Recieving test data count: ', data.value)
                global test_data_count
                test_data_count = data.value
        socketio.sleep(0.001)

def data_thread():
    # Data thread is respondsible for sending the data request command to the serial port
    print('starting data task')
    while True:
        #print('Sending data command')
        communication.get_data()
        socketio.sleep(0.01)

def state_thread():
    # State thread is respondsible for sending the state request command to the serial port
    print('starting status task')
    index = 0
    while True:
        communication.get_state()
        socketio.sleep(1)

@socketio.on('profile', namespace='/serial')
def serial_client_profile():
    communication.get_machine_profile()

@socketio.on('connect', namespace='/serial')
def serial_client_connect():
    pass

def test_data_reciever_thread():
    # Data thread is respondsible for sending the data request command to the serial port
    print('starting test_data_reciever')
    index = 0
    with test_data_lock:
        global test_data
        global test_data_count
        with open("test_data.csv","w") as f:
            print('file opened')
            writer = csv.writer(f, delimiter=',', quotechar='|')
            writer.writerow(['index','time(us)', 'position(um)', 'force(mN)'])
            communication.get_test_data_count()
            while test_data_count == 0:
                socketio.sleep(0.1)
            print(f'test data count: {test_data_count}')
            data_count = test_data_count
            test_data_count = 0 # reset global variable
            while data_count > index:
                print(f'getting test data: {index}')
                communication.get_test_data(index, 2)
                start_time = datetime.now()
                while test_data is None:
                    time_delta = datetime.now() - start_time
                    if time_delta.total_seconds() >= 0.1:
                        print('timeout')
                        break
                    if test_data_complete:
                        print('End of data')
                        return
                    socketio.sleep(0.01)
                if test_data is None:
                    continue
                #if test_data.log != index:
                #    print(f'index mismatch: {test_data.log} != {index}')
                #    continue
                print(" size of test data is ", len(test_data))
                index += 2
                print(f'got test data: {test_data.timeus, test_data.encoderum, test_data.forcemN}')
                writer.writerow([test_data.log, test_data.timeus, test_data.encoderum, test_data.forcemN])
                socketio.emit('testdata', {'json': json.dumps(flatten_dict(test_data.getdict()))}, namespace = '/serial')
                test_data = None
                socketio.sleep(0.01)
        print("Done gathering data")

def gcode_sender_thread(filename):
    global gcode_lock
    with gcode_lock:
        print('starting gcode sender task')
        # open file
        with open(filename) as f:
            lines = f.readlines()
        # send lines
        global gcode_ack
        for line in lines:
            print(f'sending gcode: {line}')
            code = re.findall(r'[G]\d+', line)
            xcord = re.findall(r'[X]\d+\.\d+', line)
            feedrate = re.findall(r'[F]\d+', line)
            if not xcord or not feedrate:
                print("No xcord or feedrate")
                continue
            x = round(float(xcord[0][1:])*1000)
            f = int(feedrate[0][1:])
            print("G{} - X{} - F{}".format(code, x, f))
            if code[0] == "G1":
                x = -x
            command = MotionCommand()
            command.steps = x
            command.feedrate = f
            command.last = False
            gcode_ack = "NONE"
            communication.set_motion_command(command)
            timeout = time.time() + 10   # 10 seconds from now
            while True:
                if gcode_ack == "OK":
                    break
                elif gcode_ack == "FAIL":
                    communication.set_motion_command(command) # try again
                elif gcode_ack == "BUSY":
                    communication.set_motion_command(command) # try again
                    pass
                elif gcode_ack == "NONE":
                    if time.time() > timeout:
                        print("Timeout")
                        return
                socketio.sleep(0.1)
        last_command = MotionCommand()
        last_command.steps = 0
        last_command.feedrate = 0
        last_command.last = True
        communication.set_motion_command(last_command)
        print('gcode sender task finished successfully')