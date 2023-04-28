from app import app, socketio, serial_thread, state_thread, data_thread
import subprocess

@app.route("/restart")
def restart():
    subprocess.run("shutdown -r 0", shell=True, check=True)
    return "Restarting"

@app.route("/shutdown")
def shutdown():
    subprocess.run("shutdown -h 0", shell=True, check=True)
    return "Shutting down!"

if __name__ == "__main__":
    try:
        socketio.start_background_task(target=serial_thread)
        socketio.start_background_task(target=state_thread)
        #socketio.start_background_task(target=data_thread)
        socketio.run(app, port=5000, host="0.0.0.0",
                     debug=True, use_reloader=False)
    except KeyboardInterrupt:
        pass
