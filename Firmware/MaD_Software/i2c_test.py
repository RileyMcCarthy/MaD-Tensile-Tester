import time
from SerialHelpers import MaD_Serial
import pigpio

mSerial = MaD_Serial()

data_ready = 22


def data_transmit_thread():
    print("CStarting monitor thread")
    pi = pigpio.pi()
    if not pi.connected:
        print("Can't connected to pigpio daemon")
        exit()
    # GPIO 22 set up as input. It is pulled up to stop false signals
    #pi.set_mode(data_ready, pigpio.INPUT)
    #pi.set_pull_up_down(data_ready, pigpio.PUD_OFF)

    # def read_data(gpio, level, tick):
    def read_data():
        # print("callback")
        monitorData = mSerial.getMonitorData()
        if monitorData is None:
            print("Error reading monitor data")
            return
        time = monitorData.timems/1000.0
        position = monitorData.encoderum/1000000.0
        force = monitorData.forcemN/1000.0
        setpoint = monitorData.setpoint
        print(monitorData.log)
        # print({'time': time, 'position': position,
        #       'force': force, 'setpoint': setpoint, 'log': monitorData.log})
        # socketio.emit('data', {'time': data['time'], 'position': data['position'],
        #                      'force': data['force'], 'setpoint': data['setpoint']}, namespace='/monitor')
    #pi.callback(data_ready, pigpio.FALLING_EDGE, read_data)
    try:
        while True:
            read_data()
            # time.sleep(1)
    finally:
        pass


data_transmit_thread()
