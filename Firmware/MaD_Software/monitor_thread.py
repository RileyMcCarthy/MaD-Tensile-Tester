from Helpers import machine_status_to_html
from threading import RLock, Event, Lock
from app import app, socketio
from Helpers import print_ctypes_obj
import ctypes
import pigpio
import json
import multiprocessing
from Helpers import crc8
from app import mSerial

manager = multiprocessing.Manager()
data_dict = manager.dict()

thread = None
thread_lock = RLock()

data_ready = 22


def data_transmit_thread():
    print("CStarting monitor thread")
    pi = pigpio.pi()
    if not pi.connected:
        print("Can't connected to pigpio daemon")
        exit()

    pi.set_mode(data_ready, pigpio.INPUT)
    pi.set_pull_up_down(data_ready, pigpio.PUD_OFF)

    def read_data(gpio, level, tick):
        monitorData = mSerial.getMonitorData()
        if monitorData is None:
            print("Error reading monitor data")
            return
        time = monitorData.timeus/1000000.0
        position = monitorData.encoderum/1000000.0
        force = monitorData.forcemN/1000.0
        setpoint = monitorData.setpointum/1000000.0
        # print({'time': time, 'position': position,
        #       'force': force, 'setpoint': setpoint, 'log': monitorData.log})
        # socketio.emit('data', {'time': time, 'position': position,
        #                       'force': force, 'setpoint': setpoint}, namespace='/monitor')
    pi.callback(data_ready, pigpio.FALLING_EDGE, read_data)
    try:
        while True:
            socketio.sleep(1)
    finally:
        pass


@socketio.on('connect', namespace='/monitor')
def connect():
    socketio.sleep(1)
    print("Connected to /monitor")
    global thread
    # with thread_lock:
    #    if thread is None:
    #        thread = socketio.start_background_task(
    #            data_transmit_thread)


@socketio.on('disconnect', namespace='/monitor')
def disconnect():
    pass
