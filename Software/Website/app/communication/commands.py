from .protocol import serial_read, serial_write, serial_recieve, serial_init
import ctypes
from app.definitions import MonitorDataPacket, MachineState, MachineProfile, MachineConfiguration, MachinePerformance, MotionProfile, MotionMode, MotionStatus

CMD_PING = 0
CMD_DATA = 1
CMD_STATE = 2
CMD_MPROFILE = 3
CMD_MCONFIG = 4
CMD_MPERFORMANCE = 5
CMD_MOTIONPROFILE = 6
CMD_MOTIONMODE = 7
CMD_MOTIONFUNCTION = 8
CMD_MOTIONSTATUS = 9
CMD_FLASHDATA = 10

CMD_TYPE = {
    CMD_PING: ctypes.c_uint8,
    CMD_DATA: MonitorDataPacket,
    CMD_STATE: MachineState,
    CMD_MPROFILE: MachineProfile,
    CMD_MCONFIG: MachineConfiguration,
    CMD_MPERFORMANCE: MachinePerformance,
    CMD_MOTIONPROFILE: MotionProfile,
    CMD_MOTIONMODE: MotionMode,
    CMD_MOTIONFUNCTION: ctypes.c_int32,
    CMD_MOTIONSTATUS: MotionStatus,
}

def start():
    return serial_init(CMD_TYPE)

def get_ping():
    serial_read(CMD_PING)

def get_data():
    serial_read(CMD_DATA)

def get_state():
    serial_read(CMD_STATE)

def process_recieved():
    # read data from serial
    res = serial_recieve()
    if res is None:
        return None
    
    # Parse the data into the correct structure and add it to the dictionary
    cmd, data = res
    packet = CMD_TYPE[cmd].from_buffer_copy(data)

    print("Recieved(" + str(cmd)+"): " + str(packet))
    return packet 