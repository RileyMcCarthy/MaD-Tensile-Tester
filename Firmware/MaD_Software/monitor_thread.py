from Helpers import machine_status_to_html
from threading import RLock, Event, Lock
from app import app, socketio
from Helpers import print_ctypes_obj
from definitions.MonitorDefinition import MonitorData
import spidev
import RPi.GPIO as GPIO
import ctypes
thread = None
thread_lock = RLock()
thread_event = Event()
clients = 0
monitorData = None

# too much processing for a thread, need to use multiprocessing


def data_thread(queue):
    print("CStarting monitor thread")
    GPIO.setmode(GPIO.BCM)
    # GPIO 10 set up as input. It is pulled up to stop false signals
    GPIO.setup(22, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    # We only have SPI bus 0 available to us on the Pi
    bus = 0
    # Device is the chip select pin. Set to 0 or 1, depending on the connections
    device = 0
    # Enable SPI
    spi = spidev.SpiDev()
    spi.close()
    # Open a connection to a specific bus and device (chip select pin)
    spi.open(bus, device)
    # Set SPI speed and mode
    spi.max_speed_hz = 100000
    spi.mode = 0
    try:
        f = open("data_cache.csv", "w")
        f.write("time,position,force,setpoint\n")
        while True:
            if GPIO.wait_for_edge(22, GPIO.FALLING):
                socketio.sleep(1)
                monitorData = MonitorData.from_buffer_copy(
                    bytearray(spi.readbytes(ctypes.sizeof(MonitorData))))
                # print_ctypes_obj(monitorData)
                f.write("time:{},position:{},force{},setpoint{}\n".format(
                    monitorData.timems/1000.0, monitorData.position, monitorData.force, monitorData.setpoint))
                # socketio.emit('data', {"time": monitorData.timems/1000.0, "position": monitorData.position,
                #                       "force": monitorData.force, "setpoint": monitorData.setpoint/1000.0}, namespace='/monitor')
    finally:
        f.close()


@socketio.on('connect', namespace='/monitor')
def connect():
    socketio.sleep(1)
    print("Connected to /monitor")
    #global thread
    # with thread_lock:
    #    if thread is None:
    #        thread = socketio.start_background_task(
    #            task_thread, app)


@socketio.on('disconnect', namespace='/monitor')
def disconnect():
    pass
