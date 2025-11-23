#include <WiFi.h>
#include <PubSubClient.h>
#include "cred.h"
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 27
const int TRIG_PIN = 25;
const int ECHO_PIN = 14;
long duration;
float cms;

unsigned long previous = 0;
const long stop = 2000;

const char *mqtt_broker = "165.22.122.17";
const char *topic1 = "sensor/distance";
const char *topic2 = "sensor/temperature";
const char *topic3 = "sensor/humidity";
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
    return String(cms) + " Cm";
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
    return String(DHT.temperature) + " °C";
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
    return String(DHT.humidity) + " %";
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

  unsigned long now = millis();
  if (now - previous > stop) {
    previous = now;

    String dst = getDis();
    String temp= getTemp();
    String hum= getHumid();
    client.publish(topic1, dst.c_str());
    client.publish(topic2, temp.c_str());
    client.publish(topic3, hum.c_str());
  }
}
