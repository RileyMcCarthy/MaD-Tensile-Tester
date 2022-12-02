import time
from SerialHelpers import MaD_Serial, CMD_DATA
from Helpers import print_ctypes_obj
import pigpio

mSerial = MaD_Serial()

data_ready = 22

lasttime = 0

try:
    while True:
        #mSerial.parse_packet()
        #monitorData = mSerial.rx.pop(CMD_DATA, None)
        #if monitorData is not None:
        #    print(str(monitorData.timeus)+" "+str(monitorData.log))
        time.sleep(1)
finally:
    pass


def data_transmit_thread():
    print("CStarting monitor thread")
    pi = pigpio.pi()
    if not pi.connected:
        print("Can't connected to pigpio daemon")
        exit()
    # GPIO 22 set up as input. It is pulled up to stop false signals
    pi.set_mode(data_ready, pigpio.INPUT)
    pi.set_pull_up_down(data_ready, pigpio.PUD_UP)

    def read_data(gpio, level, tick):
        # def read_data():
        # print("callback")
        global lasttime
        monitorData = mSerial.getMonitorData()
        if monitorData is None:
            print("Error reading monitor data")
            return
        diff = monitorData.timeus-lasttime
        if diff > 0:
            print(str(monitorData.log)+" " + str(1000000/diff))
        else:
            print("error")
        lasttime = monitorData.timeus

        # print({'time': time, 'position': position,
        #       'force': force, 'setpoint': setpoint, 'log': monitorData.log})
        # socketio.emit('data', {'time': data['time'], 'position': data['position'],
        #                      'force': data['force'], 'setpoint': data['setpoint']}, namespace='/monitor')
    pi.callback(data_ready, pigpio.FALLING_EDGE, read_data)
    try:
        while True:
            # read_data()
            time.sleep(1)
    finally:
        pass


# data_transmit_thread()
