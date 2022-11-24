from Helpers import machine_status_to_html
from threading import RLock, Event, Lock
from app import app, socketio
from Helpers import print_ctypes_obj
from definitions.MonitorDefinition import MonitorData, DataPacket
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

data_ready = 3


def data_thread(data):
    print("CStarting monitor thread")
    pi = pigpio.pi()
    if not pi.connected:
        print("Can't connected to pigpio daemon")
        exit()
    # GPIO 22 set up as input. It is pulled up to stop false signals
    pi.set_mode(data_ready, pigpio.INPUT)
    pi.set_pull_up_down(data_ready, pigpio.PUD_OFF)

    def read_data(gpio, level, tick):
        monitorData = mSerial.getMonitorData()
        if monitorData is None:
            print("Error reading monitor data")
            return
        data['time'] = monitorData.timems/1000.0
        data['position'] = monitorData.encoderum/1000000.0
        data['force'] = monitorData.forcemN/1000.0
        data['setpoint'] = monitorData.setpoint
        # print({'time': data['time'], 'position': data['position'],
        #       'force': data['force'], 'setpoint': data['setpoint']})
    pi.callback(data_ready, pigpio.FALLING_EDGE, read_data)
    try:
        while True:
            socketio.sleep(1)
    finally:
        pass


def data_thread_old(data):
    print("CStarting monitor thread")
    pi = pigpio.pi()
    if not pi.connected:
        print("Can't connected to pigpio daemon")
        exit()
    # GPIO 22 set up as input. It is pulled up to stop false signals
    pi.set_mode(22, pigpio.INPUT)
    pi.set_pull_up_down(22, pigpio.PUD_OFF)
    # Enable SPI
    spi_handler = pi.spi_open(0, 100000, 0)  # Channel, baud, flags

    def read_spi(gpio, level, tick):
        num_bytes, bytes_in = pi.spi_read(
            spi_handler, ctypes.sizeof(DataPacket))
        if num_bytes != ctypes.sizeof(DataPacket):
            print("Error reading SPI, bytes to not match. Got " +
                  str(num_bytes)+" bytes, expected "+str(ctypes.sizeof(DataPacket)))
            return
        dataPacket = DataPacket.from_buffer_copy(bytearray(bytes_in))
        if dataPacket.crc != crc8(bytearray(dataPacket.data)):
            print("CRC Error: " + str(bytearray(bytes_in)))
            return
        monitorData = dataPacket.data
        data['time'] = monitorData.timems/1000.0
        data['position'] = monitorData.encoderum/1000000.0
        data['force'] = monitorData.forcemN/1000.0
        data['setpoint'] = monitorData.setpoint
        # print({'time': data['time'], 'position': data['position'],
        #       'force': data['force'], 'setpoint': data['setpoint']})
    pi.callback(22, pigpio.FALLING_EDGE, read_spi)
    try:
        while True:
            socketio.sleep(1)
    finally:
        pass


def data_transmit_thread(data):
    print("CStarting monitor thread")
    pi = pigpio.pi()
    if not pi.connected:
        print("Can't connected to pigpio daemon")
        exit()
    # GPIO 22 set up as input. It is pulled up to stop false signals
    pi.set_mode(data_ready, pigpio.INPUT)
    pi.set_pull_up_down(data_ready, pigpio.PUD_OFF)

    def read_data(gpio, level, tick):
        monitorData = mSerial.getMonitorData()
        if monitorData is None:
            print("Error reading monitor data")
            return
        time = monitorData.timems/1000.0
        position = monitorData.encoderum/1000000.0
        force = monitorData.forcemN/1000.0
        setpoint = monitorData.setpoint
        print({'time': time, 'position': position,
               'force': force, 'setpoint': setpoint})
        # socketio.emit('data', {'time': data['time'], 'position': data['position'],
        #                      'force': data['force'], 'setpoint': data['setpoint']}, namespace='/monitor')
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
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(
                data_transmit_thread, data_dict)


@socketio.on('disconnect', namespace='/monitor')
def disconnect():
    pass
