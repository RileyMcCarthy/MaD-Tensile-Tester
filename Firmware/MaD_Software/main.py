from app import app, socketio
from status_thread import *
from monitor_thread import *

if __name__ == "__main__":
    app.debug = True
    try:
        socketio.run(app, port=5000, host="0.0.0.0",
                     debug=True, use_reloader=True)
    except KeyboardInterrupt:
        pass
