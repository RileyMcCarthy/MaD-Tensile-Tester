from flask import render_template, request
from queue import Queue
from .helpers import ctypes_to_dict
import json
from app import app, socketio
import app.communication as communication
from app.communication.commands import CMD_AWK, CMD_TESTDATA,CMD_TESTDATA_COUNT,CMD_STATE,CMD_DATA,CMD_MPROFILE,CMD_MOVE
from collections.abc import MutableMapping
from .helpers import flatten_dict
from threading import Lock
import re
import time
import csv
from datetime import datetime


gcode_lock = Lock()
gcode_ack = "NONE"
gcode_ack_lock = Lock()

test_data_lock = Lock()
test_data = None
test_data_count = 0
test_data_complete = False

def emit_notification(type, message, timeout = None):
    if type == 'error':
        if timeout is None:
            timeout = 10000
        app.logger.error(message)
    elif type == 'warning':
        if timeout is None:
            timeout = 4000
        app.logger.warn(message)
    elif type == 'success':
        if timeout is None:
            timeout = 4000
        app.logger.info(message)
    else:
        app.logger.info(message)
    notification = {'type': type, 'message': message, 'timeout': timeout}
    socketio.emit('notification', {'json': json.dumps(notification)}, namespace = '/serial')

def ack_handler(data_json):
    if 'awk' not in data_json:
        app.logger.error("Invalid ack recieved!")
        return
    if data_json['cmd'] == CMD_MPROFILE:
        if data_json['awk'] == "OK":
            emit_notification('success', 'Motion Profile Uploaded Successfully')
        elif data_json['awk'] == "FAIL":
            emit_notification('error', 'Failed to upload motion profile')
    elif data_json['cmd'] == CMD_MOVE:
        global gcode_ack
        global gcode_ack_lock
        with gcode_ack_lock:
            gcode_ack = data_json['awk']
            print('Recieving ack command:"', gcode_ack,'"')
def serial_thread(serial_port, serial_baud):
    # Serial thread is responsible for reading the serial port and sending the data to the client
    print('starting serial task')    
    while not communication.start(serial_port, serial_baud):
        emit_notification('warning', 'Failed to connect to device!', 4000)
        socketio.sleep(5)
    emit_notification('success', 'Connected to device!')
    while True:
        try:
            res = communication.process_recieved()
        except:
            app.logger.error("Failed to process incomming data!")
        if res is not None:
            cmd, data_json = res
            #print(f'got data: {data_json}')
            if cmd == CMD_STATE:
                socketio.emit('state', {'json': json.dumps(data_json)}, namespace = '/serial')
            elif cmd == CMD_DATA:  
                socketio.emit('data', {'json': json.dumps(data_json)}, namespace = '/serial')
            elif cmd == CMD_MPROFILE:
                socketio.emit('profile', {'json': json.dumps(data_json)}, namespace = '/serial')
            elif cmd == CMD_AWK:
                ack_handler(data_json)
            elif cmd == CMD_TESTDATA:
                print('Recieving test data: ', data_json)
                global test_data_complete
                global test_data
                test_data_complete = data_json.Test_Data is None
                test_data = data_json.Test_Data
                print('got:',test_data,' of size ',len(test_data))
                #socketio.emit('test_data', {'json': json.dumps(data_integer)}, namespace = '/serial')
            elif cmd == CMD_TESTDATA_COUNT:
                #print('Recieving test data count: ', data.value)
                global test_data_count
                test_data_count = data_json.test_count
        socketio.sleep(0.01)

def data_thread():
    # Data thread is respondsible for sending the data request command to the serial port
    print('starting data task')
    while True:
        #print('Sending data command')
        communication.get_data()
        socketio.sleep(0.2)

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
    global test_data_lock
    with test_data_lock:
        global test_data
        global test_data_count
        with open("test_data.csv","w") as f:
            print('file opened')
            writer = csv.writer(f, delimiter=',', quotechar='|')
            writer.writerow(['index','time(us)', 'position(um)', 'force(mN)'])
            communication.get_test_data_count()
            while test_data_count == 0:
                socketio.sleep(0.01)
            print(f'test data count: {test_data_count}')
            data_count = test_data_count
            test_data_count = 0 # reset global variable
            while data_count > index:
                block_size = 20
                print(f'getting test data: {index}')
                communication.get_test_data(index, block_size)
                start_time = datetime.now()
                while test_data is None:
                    time_delta = datetime.now() - start_time
                    if time_delta.total_seconds() >= 1:
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
                index += len(test_data)
                # Ilterate through list test_data
                for i in range(len(test_data)):
                    print(f'got test data: {test_data[i].log, test_data[i].encoderum, test_data[i].forcemN}')
                    writer.writerow([test_data[i].log, test_data[i].timeus, test_data[i].encoderum, test_data[i].forcemN])
                    socketio.emit('testdata', {'json': json.dumps(flatten_dict(test_data[i].getdict()))}, namespace = '/serial')
                #print(f'got test data: {test_data.timeus, test_data.encoderum, test_data.forcemN}')
                #writer.writerow([test_data.log, test_data.timeus, test_data.encoderum, test_data.forcemN])
                #socketio.emit('testdata', {'json': json.dumps(flatten_dict(test_data.getdict()))}, namespace = '/serial')
                test_data = None
                socketio.sleep(0.01)
        print("Done gathering data")

def gcode_to_dict(gcode_string):
    valid_commands = ['G', 'X', 'F', 'P']
    gcode_dict = {}
    words = gcode_string.split()
    
    for word in words:
        command = word[0]
        if command not in valid_commands:
            emit_notification('warning', f'Invalid gcode command: {command}, valid commands are: {valid_commands}')
            continue
        value_str = word[1:]
        if '.' in value_str:  # Check if value contains a decimal point
            value = float(value_str)
        else:
            value = int(value_str)
        gcode_dict[command] = value
    return gcode_dict

def gcode_sender_thread(filename):
    timeout_period = 5
    global gcode_lock
    print("aquireing gcode lock!")
    if gcode_lock.acquire(timeout = 1):
        # SHould clear test buffer first!!!
        # isolated filename from file path
        file = re.search(r'[^\\/]+$', filename).group(0)
        emit_notification("success",f'Queueing {file} Profile on Device!',timeout_period*1000)
        # open file
        with open(filename) as f:
            lines = f.readlines()
        # send lines
        global gcode_ack
        for line in lines:
            print(f'sending gcode: {line}')
            command = gcode_to_dict(line)
            global gcode_ack_lock
            with gcode_ack_lock:
                gcode_ack = "NONE"
            communication.set_motion_command(command)
            timeout = time.time() + timeout_period   # 5 seconds from now
            retries = 0
            while True:
                result = "NONE"
                with gcode_ack_lock:
                    result = gcode_ack
                if result == "OK":
                    break
                elif result == "FAIL":
                    communication.set_motion_command(command) # try again
                    retries += 1
                    if retries > 5:
                        emit_notification('error', 'Failed to send gcode, too many retries!')
                        gcode_lock.release()
                        return
                elif result == "BUSY":
                    # should wait for ok before trying again...
                    communication.set_motion_command(command) # try again
                    socketio.sleep(0.5) # slow down sending
                elif result == "NONE":
                    if time.time() > timeout:
                        emit_notification('error', 'Timeout sending gcode to device!')
                        gcode_lock.release()
                        return
                socketio.sleep(0.1)
        gcode_lock.release()
        emit_notification('success', 'Gcode sent successfully')
    else:
        emit_notification('warning', 'Unable to start gcode sender, locked!')