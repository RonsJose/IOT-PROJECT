# The Smart System integration for automobiles

The smart system integration for automobiles (SSIA), is an IoT based system that aims to improve and aid, both old and new cars alike. The primary purpose is to bring them more safety and comfort features, at a substantially lower cost. Striving towards a better world, the SSIA follows the United Nations sustainable development goals such as good health and well-being, industry innovation an infrastructure, sustainable cities and communties & responsible consumption and production.


Link to project: [https://ssia.online]


<img width="1952" height="1058" alt="image" src="https://github.com/user-attachments/assets/2a5a942a-ca8f-46a1-8f9b-ed3369e51a9d" />

# How It's Made:
**Tech used:** C/C++, HTML, CSS, JavaScript, Flask, Gunicorn, Eventlet, Nginx, PostgreSQL, SQL Alchemy, MQTT, Eclipse Mosquitto, Http, WebSockets, Socket.io, REST API's, Cloud Deployment(DigitalOcean), Linux, UART, I2C, SPI


**Hardware used:** ESP32, HCSR04, DHT 11, MikroElektronika Heart Rate 3 Click (MAX30100), Ultimate GPS V3.0, MFRC522, FS90 Micro Servo, Analog Alcohol Sensor (MQ3), Ai Thinker ESP32-CAM 


**Sensors:**:


**Server:**
The server is a cloud hosted Ubuntu virtual machine (DigitalOcean) which runs the frontend, Flask backend, PostgreSQL database and Eclipse Mosquitto MQTT broker. Nginx serves the templates/static files for the frontend & also is used as a reverse proxy , which then forwards requests to gunicorn. 


Gunicorn is a WSGI (web server gateway interface) which is responsible for running the flask app, which handles incoming and out going requests from nginx and flask. It handles incoming requests, distributes them across multiple worker processes, and returns responses back through Nginx to the client. 


To make the workers more efficiently we use eventlet which allows them to work asynchronously, i.e can handle more than one request at a time.





# Features: 
* Real-Time Dashboard: Web interface displaying live vehicle data and system status
* Proximity Detection: Ultrasonic sensor with audio (buzzer) feedback based on distance
* Driver Safety (Alcohol Detection): Detects high alcohol levels, locks doors, and sends location-based alerts via email
* Climate Monitoring: Temperature and humidity tracking for driver comfort
* GPS Tracking: Provides accurate, real-time location data
* Rear Camera Support: Live camera feed for parking and manoeuvring
* Health Monitoring: Tracks heart rate and blood oxygen levels
* RFID Access Control: Lock/unlock system with support for adding and removing authorised cards

# Optimizations

# Lessons Learned:





