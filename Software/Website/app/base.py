from flask import render_template, request
from queue import Queue
from .helpers import ctypes_to_dict
import json
from app import app, socketio
import app.communication as communication
from app.communication.commands import CMD_AWK, CMD_TESTDATA,CMD_TESTDATA_COUNT,CMD_STATE,CMD_DATA,CMD_MPROFILE,CMD_MOVE, CMD_NOTIFICATION, CMD_TEST_HEADER
from collections.abc import MutableMapping
from .helpers import flatten_dict
from threading import Lock
import re
import time
import csv
from datetime import datetime
import os

gcode_lock = Lock()
gcode_ack = "NONE"
gcode_ack_lock = Lock()

test_data_lock = Lock()
test_data = None
test_data_count = 0
test_data_complete = False

gcode_sender_thread_path = None

def emit_notification(type, message, timeout = None):
    type = type.lower()
    if type.lower() == 'error':
        if timeout is None:
            timeout = 10000
        app.logger.error(message)
    elif type.lower() == 'warning':
        if timeout is None:
            timeout = 4000
        app.logger.warn(message)
    elif type.lower() == 'success':
        if timeout is None:
            timeout = 4000
        app.logger.info(message)
    else:
        app.logger.info(message)
    try:
        trimmed_message = message.split("::", 1)[1]
    except:
        trimmed_message = message
    notification = {'type': type, 'message': trimmed_message, 'timeout': timeout}
    socketio.emit('notification', {'json': json.dumps(notification)}, namespace = '/serial')

def set_gcode_sender_thread_path(path):
    global gcode_sender_thread_path
    gcode_sender_thread_path = path

def ack_handler(data_json):
    cmd = int(data_json['cmd'])
    print('Recieved ack command:', cmd ,':', data_json['awk'])
    if 'awk' not in data_json:
        app.logger.error("Invalid ack recieved!")
        return
    if cmd == CMD_MPROFILE:
        if data_json['awk'] == "OK":
            emit_notification('success', 'Motion Profile Uploaded Successfully')
        elif data_json['awk'] == "FAIL":
            emit_notification('error', 'Failed to upload motion profile')
    elif cmd == CMD_MOVE:
        global gcode_ack
        global gcode_ack_lock
        with gcode_ack_lock:
            gcode_ack = data_json['awk']
            print('Recieving ack command:"', gcode_ack,'"')
    elif cmd == CMD_TEST_HEADER:
        if data_json['awk'] == "OK":
            emit_notification('success', 'Test is setup on Device!')
            global gcode_sender_thread_path
            if gcode_sender_thread_path is not None:
                socketio.start_background_task(target=gcode_sender_thread, filename=gcode_sender_thread_path)
                gcode_sender_thread_path = None
            else:
                print('gcode_sender_thread_path is None')
        elif data_json['awk'] == "FAIL":
            emit_notification('error', 'Failed to setup test on device')


def serial_thread(serial_port, serial_baud):
    # Serial thread is responsible for reading the serial port and sending the data to the client
    print('starting serial task')    
    while not communication.start(serial_port, serial_baud):
        emit_notification('warning', 'Failed to connect to device!', 4000)
        socketio.sleep(5)
    emit_notification('success', 'Connected to device!')
    
    while True:
        socketio.sleep(0.01)
        try:
            res = communication.process_recieved()
        except Exception as err:
            app.logger.error("Failed to process incomming data: " + str(err))
            continue
        
        if res is None:
            continue

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
            global test_data_complete
            global test_data
            test_data_complete = data_json['Test_Data'] is None
            test_data = data_json['Test_Data']
            print('got test data of size ',len(test_data))
        elif cmd == CMD_TESTDATA_COUNT:
            #print('Recieving test data count: ', data.value)
            global test_data_count
            test_data_count = data_json['test_count']
        elif cmd == CMD_NOTIFICATION:
            print("Recieving notification: ", data_json)
            emit_notification(data_json['Type'], data_json['Message'], 5000)

def data_thread(period):
    # Data thread is respondsible for sending the data request command to the serial port
    print('starting data task')
    while True:
        #print('Sending data command')
        communication.get_data()
        socketio.sleep(period)

def state_thread(period):
    # State thread is respondsible for sending the state request command to the serial port
    print('starting status task')
    index = 0
    while True:
        communication.get_state()
        socketio.sleep(period)

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
        # save data into test folder 
        path = os.path.join(app.config['TEST_FOLDER'], 'test_data.csv')
        with open(path,"w") as f:
            print('file opened')
            writer = csv.writer(f, delimiter=',', quotechar='|')
            writer.writerow(['Time(us)', 'Position(um)', 'Force(mN)','Setpoint(um)'])
            communication.get_test_data_count()
            timeout = time.time() + 5   # 5 seconds from now
            while test_data_count == 0:
                if time.time() > timeout:
                    print('timeout')
                    return
                socketio.sleep(0.01)
            print(f'test data count: {test_data_count}')
            emit_notification('success', 'Recieving test data of size' + str(test_data_count))
            data_count = test_data_count
            test_data_count = 0 # reset global variable
            while data_count > index:
                block_size = 40
                print(f'getting test data: {index}')
                communication.get_test_data(index, block_size)
                start_time = datetime.now()
                while test_data is None:
                    time_delta = datetime.now() - start_time
                    if time_delta.total_seconds() >= 5:
                        emit_notification('error', 'Failed to recieve test data, timeout')
                        break
                    if test_data_complete:
                        emit_notification('success', 'Recieved all test data')
                        return
                    socketio.sleep(0.001)
                if test_data is None:
                    continue
                index += len(test_data)
                # Ilterate through list test_data
                for i in range(len(test_data)):
                    writer.writerow([test_data[i]['Time'], test_data[i]['Position'], test_data[i]['Force'], test_data[i]['Setpoint']])
                    #socketio.emit('testdata', {'json': json.dumps(test_data[i])}, namespace = '/serial')
                test_data = None
                socketio.sleep(0.0001)
        emit_notification('success', 'Recieved all test data')
        print("Done gathering data")

def generate_gcode_from_profile(json_file_path):
    with open(json_file_path, 'r') as json_file:
        json_data = json.load(json_file)

    gcode_str = '; G-code generated from JSON\n'
    gcode_str += '; Name: ' + json_data['name'] + '\n\n'
    
    for set_data in json_data['sets']:
        gcode_str += '; Set: ' + set_data['name'] + '\n'
        for _ in range(set_data['executions']):
            gcode_str += set_data['gcode'] + '\n'
        gcode_str += '\n'
    
    return gcode_str

def remove_comments(gcode_string):
    # Regular expression pattern to match comments (lines starting with semicolon)
    pattern = r'^\s*;.*$'
    
    # Use re.MULTILINE to match comments on multiple lines
    return re.sub(pattern, '', gcode_string, flags=re.MULTILINE)

def gcode_to_dict(gcode_string):
    valid_commands = {'G': int, 'X': float, 'F': float, 'P': int}
    gcode_dict = {}
    gcode_string = remove_comments(gcode_string)
    words = gcode_string.split()
    
    for word in words:
        command = word[0]
        if command not in valid_commands:
            emit_notification('warning', f'Invalid gcode command: {command}, valid commands are: {valid_commands}')
            continue
        value_str = word[1:]
        gcode_dict[command] = valid_commands[command](value_str)
    if 'G' not in gcode_dict:
        return None
    return gcode_dict

def send_gcode(line, timeout_period=5):
    print(f'sending gcode: {line}')
    command = gcode_to_dict(line)
    if command is None:
        return "SKIP"
    global gcode_ack
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
            return command
        elif result == "FAIL":
            communication.set_motion_command(command) # try again
            retries += 1
            if retries > 5:
                emit_notification('error', 'Failed to send gcode, too many retries!')
                return None
        elif result == "BUSY":
            # should wait for ok before trying again...
            communication.set_motion_command(command) # try again
            socketio.sleep(1) # slow down sending
        elif result == "NONE":
            if time.time() > timeout:
                emit_notification('error', 'Timeout sending gcode to device!')
                return
        socketio.sleep(0.1)

def gcode_sender_thread(filename):
    start_command = "G90"
    end_command = "G122"
    timeout_period = 5
    global gcode_lock
    print("aquireing gcode lock!")
    if gcode_lock.acquire(timeout = 1):
        # SHould clear test buffer first!!!
        # isolated filename from file path
        file = re.search(r'[^\\/]+$', filename).group(0)
        emit_notification("success",f'Queueing {file} Profile on Device!',timeout_period*1000)
        # open file
        gcode_str = generate_gcode_from_profile(filename)
        # send lines
        global gcode_ack
        send_gcode(start_command)
        for line in gcode_str.splitlines():
            if send_gcode(line) is None:
                gcode_lock.release()
                continue

        send_gcode(end_command)
        gcode_lock.release()
        emit_notification('success', 'Gcode sent successfully')
    else:
        emit_notification('warning', 'Unable to start gcode sender, locked!')