from flask import Flask
from config import Config
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_socketio import SocketIO

UPLOAD_FOLDER = './uploads'
ALLOWED_EXTENSIONS = {'gcode', 'nc', 'txt'}



app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config.from_object(Config)
db = SQLAlchemy(app)
migrate = Migrate(app, db)
socketio = SocketIO(app)
#camera = cv2.VideoCapture(-1)

from app import base, status, settings, upload, data
from app.base import serial_thread, state_thread, data_thread, test_data_reciever_thread