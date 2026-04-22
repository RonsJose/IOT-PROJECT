# The Smart System Integration for Automobiles

The smart system integration for automobiles (SSIA), is an IoT based system that aims to improve and aid, both old and new cars alike. The primary purpose is to bring them more safety and comfort features, at a substantially lower cost. Striving towards a better world, the SSIA follows the United Nations sustainable development goals such as good health and well-being, industry innovation an infrastructure, sustainable cities and communties & responsible consumption and production.


Link to project: [https://ssia.online]


<img width="1952" height="1058" alt="image" src="https://github.com/user-attachments/assets/2a5a942a-ca8f-46a1-8f9b-ed3369e51a9d" />

# How It's Made:
**Software used:** C/C++, HTML, CSS, JavaScript, Flask, Gunicorn, Eventlet, Nginx, PostgreSQL, SQL Alchemy, MQTT, Eclipse Mosquitto, HTTP, WebSockets, Socket.io, REST API's, Cloud Deployment(DigitalOcean), Linux, UART, I2C, SPI 


**Hardware used:** 
ESP32, HC-SR04, DHT 11, MikroElektronika Heart Rate 3 Click (MAX30100), Ultimate GPS V3.0, MFRC522, FS90 Micro Servo, Analog Alcohol Sensor (MQ3), Ai Thinker ESP32-CAM, KingState KPE-522


**Sensors:**

* HC-SR04, the distance sensor it has two ultra sonic transducers which act as a transmitter and a receiver. After the sound waves are fired from the transmitter and comes into contact with
  an object te sound waves are reflected back to the receiver. The time it takes to travel back is what we use to calculate the distance since we know the speed of sound, it works between 2
  400cm. 

* DHT 11,

  * Consists of two metal electrodes with a special moisture absorbing material between the two electrodes. When the air gets more humid the material absorbs more water vapour from the air and this causes it to release ions. The ions cause the the dielectric constant of the material to change, thus in turn changing        the capacitance between the eletrodes. By measuring this change the sensor can figure out the relative humidity in the air.
    
    * Humidity range  20-80%
      
  * Negative Temperature Coefficient thermistor, a thermistor is basically a special type of resistor that changes its resistance significantly when the temperature changes.

     * Temperature range 0-50°C

* MikroElektronika Heart Rate 3 Click (MAX30100), Uses a method called Photoplethysmography which uses two leds, the red and IR led. The leds flashes light into your finger.
  * Your finger absorbs IR light differently depending on how full the arteries are. Every heartbeat pumps more blood which equals more absorbtion which means theres less light reflected. And the photodiode measures the reflected light intensity, there also are filters which help to remove noise. And a digital value is sent via I2C to the esp32.
  
  * Blood with oxygen and without oxygen absorb red light differently. Oxygenated blood absorbs more IR light and less red light. Deoxygenated blood absorbs more red light and less IR light, so the sensor measures the ratio between red/ ir to estimate the spo2 value.

* Ultimate GPS V3.0, Uses trilateration and listens to a minimum of 3-4 satelites to calculate its position. Each satelite has an internal clock that is constantly being broadcast, which can be used to calculate the distance to the satelite since the speed of RF is known. Using the distance of the satelite you can create a sphere around it. The intersecting point of all spheres is your current location.

* MFRC522 RFID module, Reads passive 13.56 MHz tags by generating an RF field that powers the tag, detects its modulated response, and sends the tag’s unique ID to the ESP32 via SPI.

* FS90 Micro Servo, Uses a potentiometer to sense position aka the current angle, then uses a control circuit to compare the desired value to the current value. Controlled using PWM.

* Analog Alcohol Sensor (MQ3), When a SnO2 semiconductor layer is heated to a high temperature, oxygen is adsorbed on the surface.

  * When the air is clean, electrons from the conduction band of tin dioxide are attracted to oxygen molecules. This creates an electron depletion layer just beneath the surface of the SnO2 particles, forming a potential barrier. As a result, the SnO2 film becomes highly resistive and prevents electric current flow.

  * In the presence of alcohol, however, the surface density of adsorbed oxygen decreases as it reacts with the alcohol, lowering the potential barrier. As a result, electrons are released into the tin dioxide, allowing current to freely flow through the sensor.

* Ai Thinker ESP32-CAM, The OV2640 sensor on the camera sees light throught the lens, and each pixel converts the light into an electrical signal. The intensity of the light dictates the value of the voltage and the value of the voltage dictages the digital pixel values. This is what creates a raw image. It is then compressed to a JPEG and sent to the ESP32 and its stored in the PSRAM. Then it's sent to an endpoint on the server via a HTTP POST.

* KingState KPE-522, A piezo buzzer is an electromechanical device that converts electrical signals into sound using the piezoelectric effect. We can turn it on and off by sending high and low signals. We use the buzzer as a audio cue for how close the vehicle is to an object which is determined by the HC-SR04 we then depending on the distance, control the delay between the buzzer being on and off.

> [!NOTE]
> All sensors with data values that need to be sent to the server are done so using MQTT via the PubSubClient library. They are subscribed to a topic and sent to the broker which forwards it to the server.

**Server:**

* The server is a cloud hosted Ubuntu virtual machine (DigitalOcean) which runs the frontend, Flask backend, PostgreSQL database and Eclipse Mosquitto MQTT broker. 
* Nginx serves the templates/static files for the frontend & also is used as a reverse proxy , which then forwards requests to gunicorn. 


* Gunicorn is a WSGI (web server gateway interface) which is responsible for running the flask app, which handles incoming and out going requests from nginx and flask. It handles incoming requests, distributes them across multiple worker processes, and returns responses back through Nginx to the client. 


* To make the workers more efficient we use eventlet which allows them to work asynchronously, i.e can handle more than one request at a time.

* 







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





