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
            try:
                status = mSerial.getMachineState()
                if status is not None:
                    html = machine_status_to_html(status)
                    socketio.emit('data', {'html': html}, namespace='/status')
            except:
                pass
            socketio.sleep(1)
    finally:
        pass


@socketio.on('connect', namespace='/status')
def connect():
    socketio.sleep(1)
    global clients
    clients += 1
    print("Device connected to status socket: "+str(clients))
    global thread
    with thread_lock:
        if thread is None:
            thread_event.set()
            thread = socketio.start_background_task(
                task_thread)
        else:
            print("Status thread already running")


@socketio.on('disconnect', namespace='/status')
def disconnect():
    global clients
    clients -= 1
    print("Device disconnected from status socket: "+str(clients))
    if clients <= 0:
        global thread
        with thread_lock:
            thread_event.clear()
            thread.join()
            thread = None
