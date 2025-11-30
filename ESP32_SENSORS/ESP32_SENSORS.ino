#include <WiFi.h>
#include <PubSubClient.h>
#include "cred.h"
#include <DFRobot_DHT11.h>
#include <Adafruit_GPS.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 27
const int TRIG_PIN = 25;
const int ECHO_PIN = 14;
long duration;
float cms;
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false

uint32_t timer = millis();
unsigned long previous = 0;
const long stop = 2000;

const char *mqtt_broker = "165.22.122.17";
const char *topic1 = "sensor/distance";
const char *topic2 = "sensor/temperature";
const char *topic3 = "sensor/humidity";
const char *topic4 = "sensor/longitude";
const char *topic5 = "sensor/latitude";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

String getDis() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  cms = (duration / 2) / 29.1;
  if (isnan(cms)) {
    Serial.println("Failed to get reading from HCRS04");
    return "--";
  } else {
    return String(cms);
  }
}

String getTemp() {
  DHT.read(DHT11_PIN);
  if (isnan(DHT.temperature)) {
    Serial.println("Failed to get reading from DHT T");
    return "--";
  } else {
    Serial.print("temp:");
    Serial.print(DHT.temperature);
    return String(DHT.temperature);
  }
}

String getHumid() {
  DHT.read(DHT11_PIN);
  if (isnan(DHT.humidity)) {
    Serial.println("Failed to get reading from DHT H");
    return "--";
  } else {
    Serial.print("  humi:");
    Serial.println(DHT.humidity);
    return String(DHT.humidity);
  }
}

String getLatitude() {
  if (GPS.fix) {
    return String(GPS.latitudeDegrees,6);
  } else {
    return "0";
  }
}

String getLongitude() {
  if (GPS.fix) {
    return String(GPS.longitudeDegrees,6);
  } else {
    return "0";
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the network");

  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);

  client.setServer(mqtt_broker, mqtt_port);
}


void loop() {
  // put your main code here, to run repeatedly:
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.println("Connecting to mqtt server\n");
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT server");
    } else {
      Serial.println("Failed to connect ");
      Serial.print(client.state());
      delay(1000);
    }
  }
  client.loop();

  while (GPSSerial.available()) {
    char c = GPS.read();
    if (GPS.newNMEAreceived()) {
      if (!GPS.parse(GPS.lastNMEA())) {
        continue;
      }
    }
  }

  unsigned long now = millis();
  if (now - previous > stop) {
    previous = now;

    String dst = getDis();
    String temp = getTemp();
    String hum = getHumid();
    String lat = getLatitude();
    String lon = getLongitude();
    client.publish(topic1, dst.c_str());
    client.publish(topic2, temp.c_str());
    client.publish(topic3, hum.c_str());
    client.publish(topic4,lon.c_str());
    client.publish(topic5,lat.c_str());
  }
}
