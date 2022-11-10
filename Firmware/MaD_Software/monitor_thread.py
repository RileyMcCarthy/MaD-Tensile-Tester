from Helpers import machine_status_to_html
from threading import RLock, Event, Lock
from app import socketio, mSerial

thread = None
thread_lock = RLock()
thread_event = Event()
clients = 0


def task_thread():
    try:
        while thread_event.is_set():
            socketio.sleep(0.2)
            if mSerial.started == False:
                print("serial has not been started")
                continue
            data = mSerial.getMonitorData()
            if data is not None:
                socketio.emit('data', {"time": data.timems/1000.0, "position": data.position,
                              "force": data.force, "setpoint": data.setpoint/1000.0}, namespace='/monitor')
    finally:
        pass


@socketio.on('connect', namespace='/monitor')
def connect():
    socketio.sleep(1)
    global clients
    clients += 1
    print("Device connected to monitor socket: "+str(clients))
    global thread
    with thread_lock:
        if thread is None:
            thread_event.set()
            thread = socketio.start_background_task(
                task_thread)


@socketio.on('disconnect', namespace='/monitor')
def disconnect():
    global clients
    clients -= 1
    print("Device disconnected from monitor socket: "+str(clients))
    if clients <= 0:
        global thread
        with thread_lock:
            thread_event.clear()
            thread.join()
            thread = None
