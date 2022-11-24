from multiprocessing import Process
import multiprocessing
from app import app, socketio
from status_thread import *
from monitor_thread import data_thread, data_dict

if __name__ == "__main__":
    p = Process(target=data_thread, args=(data_dict,))
    p.start()
    app.debug = True
    try:
        socketio.run(app, port=5000, host="0.0.0.0",
                     debug=True, use_reloader=True)
    except KeyboardInterrupt:
        p.join()
        pass
