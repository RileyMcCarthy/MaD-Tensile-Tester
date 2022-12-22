from multiprocessing import Process, Event, Lock
from app import app, socketio, db
from status_thread import connect, disconnect
from monitor_thread import data_thread
from serial_thread import *

#import SQLAlchmey definitions
from definitions import *

if __name__ == "__main__":
    with app.app_context():
        db.create_all()
    serial_quit = Event()
    p = start_serial_thread(serial_quit)
    p.start()
    #app.debug = True
    try:
        socketio.run(app, port=5000, host="0.0.0.0",
                     debug=True)
    except KeyboardInterrupt:
        serial_quit.set()
        p.join()
