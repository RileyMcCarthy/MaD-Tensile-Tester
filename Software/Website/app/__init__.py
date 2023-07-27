from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_socketio import SocketIO

UPLOAD_FOLDER = './uploads'
ALLOWED_EXTENSIONS = {'gcode', 'nc', 'txt'}



app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
socketio = SocketIO(app)
#camera = cv2.VideoCapture(-1)

from app import base, status, settings, upload, data, create
from app.base import serial_thread, state_thread, data_thread, test_data_reciever_thread, emit_notification