from Helpers import machine_status_to_html
from threading import RLock, Event, Lock
from app import app, socketio
from Helpers import print_ctypes_obj
import ctypes
import pigpio
import json
import multiprocessing
from Helpers import crc8
import SerialHelpers
import time

manager = multiprocessing.Manager()
data_dict = manager.dict()

thread = None
thread_lock = RLock()

data_ready = 22


def data_thread(serial, quit):
    # Thread to manage the serial port
    while not quit.is_set():
        try:
            serial.parse_packet()
            monitorData = serial.rx.pop(serial.CMD_DATA, None)
            if monitorData is not None:
                pass
                # print(str(monitorData.timeus)+" "+str(monitorData.log))
        except Exception as error:
            print("Exception in serial thread: " + str(error))


@socketio.on('connect', namespace='/monitor')
def connect():
    socketio.sleep(1)
    print("Connected to /monitor")
    global thread
    # with thread_lock:
    #    if thread is None:
    #        thread = socketio.start_background_task(
    #            manage_serial, mSerial)


@socketio.on('disconnect', namespace='/monitor')
def disconnect():
    pass
