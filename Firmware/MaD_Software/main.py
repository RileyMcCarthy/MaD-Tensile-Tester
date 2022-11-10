from app import app, socketio
from status_thread import *
from monitor_thread import *

if __name__ == "__main__":
    app.debug = True
    socketio.run(app, port=5000, debug=True, use_reloader=True)
