from .protocol import serial_read, serial_write, serial_recieve, serial_init 
import ctypes
from app.definitions.Motion import MotionCommand
from app.definitions.Monitor import MonitorDataPacket, MonitorData
from app.definitions.StateMachineDefinition import MachineState, MotionMode, MotionStatus
from app.definitions.JSON import MachineProfile, MachineConfiguration, MachinePerformance
from .helpers import convert_to_bytes
CMD_PING = 0
CMD_DATA = 1
CMD_STATE = 2
CMD_MPROFILE = 3
CMD_MCONFIG = 4
CMD_MPERFORMANCE = 5
CMD_MOTIONMODE = 7
CMD_MOTIONFUNCTION = 8
CMD_MOTIONSTATUS = 9
CMD_COMMAND = 10
CMD_AWK = 11
CMD_TESTDATA = 12
CMD_TESTDATA_COUNT = 13

CMD_TYPE = {
    # command: [request type, response type]
    CMD_PING: [ctypes.c_uint8,ctypes.c_uint8],
    CMD_DATA: [MonitorDataPacket,MonitorDataPacket],
    CMD_STATE: [MachineState,MachineState],
    CMD_MPROFILE: [MachineProfile,MachineProfile],
    CMD_MCONFIG: [MachineConfiguration,MachineConfiguration],
    CMD_MPERFORMANCE: [MachinePerformance,MachinePerformance],
    CMD_MOTIONMODE: [ctypes.c_int32,ctypes.c_int32],
    CMD_MOTIONFUNCTION: [ctypes.c_int32,ctypes.c_int32],
    CMD_MOTIONSTATUS: [ctypes.c_int32,ctypes.c_int32],
    CMD_COMMAND: [MotionCommand,MotionCommand],
    CMD_AWK: [ctypes.c_uint8,ctypes.c_uint8],
    CMD_TESTDATA: [ctypes.c_uint32,MonitorData]
}

def start():
    return serial_init()

def get_ping():
    serial_read(CMD_PING)

def get_data():
    serial_read(CMD_DATA)

def get_state():
    serial_read(CMD_STATE)

def get_motion_mode():
    serial_read(CMD_MOTIONMODE)

def set_state(state):
    serial_write(CMD_STATE, state)

def set_motion_mode(mode):
    serial_write(CMD_MOTIONMODE, CMD_TYPE[CMD_MOTIONMODE][0](mode))

def set_motion_status(status):
    serial_write(CMD_MOTIONSTATUS, CMD_TYPE[CMD_MOTIONSTATUS][0](status))

def set_machine_profile(profile):
    serial_write(CMD_MPROFILE, profile)

def set_motion_command(command):
    serial_write(CMD_COMMAND, command)

def get_machine_profile():
    serial_read(CMD_MPROFILE)

def get_test_data(index, count = 1):
    data = bytearray(ctypes.c_uint32(index)) # convert to bytes
    data.append(count)
    serial_read(CMD_TESTDATA, data)

def get_test_data_count():
    serial_read(CMD_TESTDATA_COUNT)

def process_recieved():
    # read data from serial
    res = serial_recieve()
    if res is None:
        return None
    cmd, data, size = res
    # process data
    if cmd == CMD_PING:
        return cmd, ctypes.c_uint8.from_buffer_copy(data)
    elif cmd == CMD_DATA:
        return cmd, MonitorDataPacket.from_buffer_copy(data)
    elif cmd == CMD_STATE:
        return cmd, MachineState.from_buffer_copy(data)
    elif cmd == CMD_MPROFILE:
        return cmd, MachineProfile.from_buffer_copy(data)
    elif cmd == CMD_MCONFIG:
        return cmd, MachineConfiguration.from_buffer_copy(data)
    elif cmd == CMD_MPERFORMANCE:
        return cmd, MachinePerformance.from_buffer_copy(data)
    elif cmd == CMD_MOTIONMODE:
        return cmd, ctypes.c_int32.from_buffer_copy(data)
    elif cmd == CMD_MOTIONFUNCTION:
        return cmd, ctypes.c_int32.from_buffer_copy(data)
    elif cmd == CMD_MOTIONSTATUS:
        return cmd, ctypes.c_int32.from_buffer_copy(data)
    elif cmd == CMD_COMMAND:
        return cmd, MotionCommand.from_buffer_copy(data)
    elif cmd == CMD_AWK:
        return cmd, ctypes.c_uint8.from_buffer_copy(data)
    elif cmd == CMD_TESTDATA:
        print("test data recieved of size: " + str(size) + " bytes "+ str(ctypes.sizeof(MonitorData)) + " bytes")
        count = int(size/ctypes.sizeof(MonitorData))
        #if size < ctypes.sizeof(MonitorData):
        #    return cmd, None
        data_list = []
        for i in range(count):
            data_list.append(MonitorData.from_buffer_copy(data, i*ctypes.sizeof(MonitorData)))
        print("test data recieved with a good size", data_list)
        return cmd, data_list
    elif cmd == CMD_TESTDATA_COUNT:
        return cmd, ctypes.c_uint32.from_buffer_copy(data)
    else:
        return None