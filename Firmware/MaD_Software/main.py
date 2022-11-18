from multiprocessing import Process
import multiprocessing
from app import app, socketio
from status_thread import *
from monitor_thread import data_thread
_pool = None

data_queue = multiprocessing.Queue()

if __name__ == "__main__":
    p = Process(target=data_thread, args=(data_queue,))
    p.start()
    app.debug = True
    try:
        socketio.run(app, port=5000, debug=True, use_reloader=True)
    except KeyboardInterrupt:
        p.join()
