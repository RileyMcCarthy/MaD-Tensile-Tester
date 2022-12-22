from app import app, socketio, serial_thread, state_thread, data_thread


if __name__ == "__main__":
    try:
        socketio.start_background_task(target=serial_thread)
        socketio.start_background_task(target=state_thread)
        socketio.start_background_task(target=data_thread)
        socketio.run(app, port=5000, host="0.0.0.0",
                     debug=True, use_reloader=False)
    except KeyboardInterrupt:
        pass
