from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)

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

@app.route("/api/test")
def api_test():
    return {"status": "ok"}

if __name__ == "__main__":
    app.run(debug=True)
