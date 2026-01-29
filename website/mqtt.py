import paho.mqtt.client as mqtt
from flask_socketio import SocketIO
from dotenv import load_dotenv 
import os

load_dotenv()
mqtt_port=9002
TOPICS = "sensor/temperature"

def mqtt_start(socketio):
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