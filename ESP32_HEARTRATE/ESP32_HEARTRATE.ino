#include <WiFi.h>
#include <PubSubClient.h>
#include "cred.h"
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define PERIOD 1000
PulseOximeter pox;
int previous1 = 0;
float heartRate = 0;
float spo2 = 0;
const long stop = 2000;
int previous = 0;

const char *mqtt_broker = "165.22.122.17";
const char *topic1 = "sensor/heartrate";
const char *topic2 = "sensor/blood";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

String getHeartRate() {
  if (isnan(heartRate)) {
    Serial.println("Failed to get reading from heart click heart");
    return "--";
  } else {
    return String(heartRate, 1);
  }
}

String getSpO2() {
  if (isnan(spo2)) {
    Serial.println("Failed to get reading from heart click spo2");
    return "--";
  } else {
    return String(spo2, 1);
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the network");

  if (!pox.begin()) {
    Serial.println("FAILED to initialize MAX30100!");
    while (1)
      ;
  } else {
    Serial.println("MAX30100 initialized successfully!");
  }

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

  pox.update();

  if (millis() - previous > PERIOD) {
    heartRate = pox.getHeartRate();
    spo2 = pox.getSpO2();
    Serial.printf("Heart Rate: %.1f bpm | SpO2: %.1f%%\n", heartRate, spo2);
    previous = millis();
  }

  unsigned long now = millis();
  if (now - previous1 > stop) {
    previous1 = now;

    String heart = getHeartRate() ;
    String blood = getSpO2();
    client.publish(topic1, heart.c_str());
    client.publish(topic2, blood.c_str());
  }
}
