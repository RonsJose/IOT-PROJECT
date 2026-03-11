from flask import Flask, render_template, request, send_file, make_response,jsonify
from dotenv import load_dotenv 
from flask_socketio import SocketIO
import threading 
import os
from mqtt import mqtt_start,lock
from flask_sqlalchemy import SQLAlchemy

load_dotenv()

app = Flask(__name__)
socketio = SocketIO(app)


UPLOAD_DIR = "uploads"
LATEST_IMAGE = os.path.join(UPLOAD_DIR, "latest.jpg")
os.makedirs(UPLOAD_DIR, exist_ok=True)

app.config["SQLALCHEMY_DATABASE_URI"] = (
    f"postgresql://{os.getenv('DB_USERNAME')}:{os.getenv('DB_PASSWORD')}"
    f"@{os.getenv('DB_HOST')}:{os.getenv('DB_PORT')}/{os.getenv('DB_NAME')}"
)

db = SQLAlchemy(app)

class User(db.Model):
    __tablename__ = "users"

    user_id = db.Column(db.Integer, primary_key=True)
    full_name = db.Column(db.String(100), nullable=False)
    email = db.Column(db.String(100))

    cards = db.relationship(
        "RFIDCard",
        backref="user",
        cascade="all, delete",
        lazy=True
    )


class RFIDCard(db.Model):
    __tablename__ = "rfid_cards"

    card_id = db.Column(db.Integer, primary_key=True)
    card_uid = db.Column(db.String(50), unique=True, nullable=False)

    user_id = db.Column(
        db.Integer,
        db.ForeignKey("users.user_id"),
        nullable=False
    )

    card_label = db.Column(db.String(50))

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

@app.route("/create-account", methods=["POST"])
def create_account():

    data = request.get_json()

    user = User(
        full_name=data.get("full_name"),
        email=data.get("email")
    )

    db.session.add(user)
    db.session.commit()

    return jsonify({
        "message": "Account created",
        "user_id": user.user_id
    })

@app.route("/register-card", methods=["POST"])
def register_card():

    data = request.get_json()

    existing = RFIDCard.query.filter_by(card_uid=data.get("card_uid")).first()
    
    if existing:
        return jsonify({"error": "Card already registered"}), 400

    card = RFIDCard(
        card_uid=data.get("card_uid"),
        user_id=data.get("user_id"),
        card_label=data.get("card_label")
    )

    db.session.add(card)
    db.session.commit()

    return jsonify({"message": "Card registered"})

@app.route("/delete-account/<int:user_id>", methods=["DELETE"])
def delete_account(user_id):

    user = User.query.get(user_id)

    if not user:
        return jsonify({"error": "User not found"}), 404

    db.session.delete(user)
    db.session.commit()

    return jsonify({"message": "Account deleted"})

@app.route("/users")
def get_users():

    users = User.query.order_by(User.user_id).all()

    result = []

    for user in users:

        card = user.cards[0] if user.cards else None

        result.append({
            "user_id": user.user_id,
            "full_name": user.full_name,
            "email": user.email,
            "card_uid": card.card_uid if card else "",
            "card_label": card.card_label if card else ""
        })

    return jsonify(result)

with app.app_context():
    db.create_all()

if __name__ == "__main__":
    socketio.run(app, host="0.0.0.0", port=8000)
