from flask import Flask, render_template, request, send_file, make_response
from dotenv import load_dotenv 
from flask_socketio import SocketIO
import threading 
import os
from mqtt import mqtt_start,lock

app = Flask(__name__)
socketio = SocketIO(app)

UPLOAD_DIR = "uploads"
LATEST_IMAGE = os.path.join(UPLOAD_DIR, "latest.jpg")
os.makedirs(UPLOAD_DIR, exist_ok=True)

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


@app.route("/set_lock", methods=["POST"])
def set_lock():
    data = request.get_json()

    if not data or "value" not in data:
        return "Missing value", 400
    
    value = data["value"]
    
    if value not in ["Open", "Closed"]:
        return "Invalid command", 400
    
    lock(value)
    return "OK", 200

@app.route("/upload", methods=["POST"])
def upload_image():
    if "image" not in request.files:
        return "No image", 400
    request.files["image"].save(LATEST_IMAGE)
    return "OK", 200

@app.route("/latest.jpg")
def latest_image():
    if not os.path.exists(LATEST_IMAGE):
        return "No image yet", 404

    response = make_response(send_file(LATEST_IMAGE, mimetype="image/jpeg"))
    response.headers["Cache-Control"] = "no-store, no-cache, must-revalidate, max-age=0"
    response.headers["Pragma"] = "no-cache"
    response.headers["Expires"] = "0"
    return response

@app.route("/accounts")
def accounts():
    return render_template('accounts.html')



if __name__ == "__main__":
    socketio.run(app, host="0.0.0.0", port=8000)
