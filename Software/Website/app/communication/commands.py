from .protocol import serial_read, serial_write, serial_recieve, serial_init, serial_test
import ctypes
from .helpers import convert_to_bytes
import json

CMD_PING = 0
CMD_DATA = 1
CMD_STATE = 2
CMD_MPROFILE = 3
CMD_MCONFIG = 4
CMD_MPERFORMANCE = 5
CMD_MOTIONMODE = 7
CMD_MOTIONFUNCTION = 8
CMD_MOTIONSTATUS = 9
CMD_MOVE= 10
CMD_AWK = 11
CMD_TESTDATA = 12
CMD_TESTDATA_COUNT = 13
CMD_MANUAL = 14
CMD_GAUGE_LENGTH = 15

def test():
    return serial_test()

def start(port, baud):
    return serial_init(port, baud)

def get_ping():
    serial_read(CMD_PING)

def get_data():
    serial_read(CMD_DATA)

def get_state():
    serial_read(CMD_STATE)

def get_motion_mode():
    serial_read(CMD_MOTIONMODE)

def get_machine_profile():
    serial_read(CMD_MPROFILE)

def get_test_data(index, count = 1):
    data = {"Index": index, "Count": count}
    serial_read(CMD_TESTDATA, json.dumps(data))

def get_test_data_count():
    serial_read(CMD_TESTDATA_COUNT)

def set_motion_command(command):
    # Converts command dict to json
    serial_write(CMD_MOVE, json.dumps(command))

def set_manual_command(command):
    # Converts command dict to json
    serial_write(CMD_MANUAL, json.dumps(command))

def set_machine_profile(profile):
    # Converts Machine Profile dict to json
    serial_write(CMD_MPROFILE, json.dumps(profile))

def set_state(state):
    # Converts state value to json
    data = {"State": state}
    serial_write(CMD_STATE, json.dumps(data))

def set_motion_mode(mode):
    # Converts mode value to json
    data = {"Mode": mode}
    serial_write(CMD_MOTIONMODE, json.dumps(data))

def set_gauge_length():
    serial_read(CMD_GAUGE_LENGTH)

def set_motion_status(status):
    # Converts status value to json
    data = {"Status": status}
    serial_write(CMD_MOTIONSTATUS, json.dumps(data))

def process_recieved():
    # read data from serial
    res = serial_recieve()
    if res is None:
        return None
    cmd, data, size = res
    decoded = data.decode("utf-8")
    json_dict = json.loads(decoded)
    return cmd, json_dict