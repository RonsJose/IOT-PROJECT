from flask import Flask, render_template, request, redirect, url_for
from dotenv import load_dotenv 
from flask_socketio import SocketIO
import threading 
import os
from mqtt import mqtt_start


app = Flask(__name__)
socketio = SocketIO(app)

threading.Thread(target=mqtt_start,args=(socketio,), daemon=True).start()

@app.route("/")
def home():
    return render_template("home.html")

@app.route("/vehicle")
def vehicle():
    return render_template("vehicle.html")

@app.route("/health")
def health():
    return render_template("health.html")

@app.route("/location")
def location():
    return render_template("location.html")

@app.route("/graphs")
def graphs():
    return render_template("graphs.html")

if __name__ == "__main__":
    socketio.run(app, host="0.0.0.0", port=8000)
