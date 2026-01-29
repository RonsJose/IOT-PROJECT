from flask import Flask, render_template, request, redirect, url_for
from dotenv import load_dotenv 
from flask_socketio import SocketIO
import paho.mqtt.client as mqtt
import threading 
import os

app = Flask(__name__)
socketio = SocketIO(app)

load_dotenv()

mqtt_port=9002

TOPICS = "sensor/temperature"

def mqtt_thread():
    client = mqtt.Client(transport="websockets")
    client.username_pw_set(os.getenv("USER"),os.getenv("PASSWORD"))

    def on_connect(client,userdata,flags,rc):
        print("Connected to broker: ",rc)
        client.subscribe(TOPICS)

    def on_message(client,userdata,message):
        payload = message.payload.decode()
        print(f"Received {message.topic}: {payload}")
        socketio.emit("mqtt_message", {"topic": message.topic, "payload": payload})

    client.on_connect = on_connect
    client.on_message = on_message

    client.tls_set()
    client.connect(os.getenv("BROKER"), mqtt_port, 60)
    client.loop_forever()

threading.Thread(target=mqtt_thread, daemon=True).start()

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
    socketio.run(app, host="0.0.0.0", port=5000)
