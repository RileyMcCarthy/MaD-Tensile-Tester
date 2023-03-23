from app import app, socketio, ALLOWED_EXTENSIONS
from flask import render_template, request, jsonify, Response, flash
from werkzeug.utils import secure_filename
from .base import test_data_reciever_thread
import os

@app.route('/testdata', methods=['GET'])
def data_page():
    socketio.start_background_task(target=test_data_reciever_thread)
    return render_template('data.html')
