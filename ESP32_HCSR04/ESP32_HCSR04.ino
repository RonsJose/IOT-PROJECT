#include <WiFi.h>
#include <PubSubClient.h>
#include "cred.h"

const int TRIG_PIN = 25;
const int ECHO_PIN = 14;
long duration;
float cms;

unsigned long previous = 0;
const long stop = 2000;

const char *mqtt_broker = "165.22.122.17";
const char *topic="sensor/distance";
const int mqtt_port=1883;

WiFiClient espClient;
PubSubClient client(espClient);

String getDis(){
digitalWrite (TRIG_PIN, LOW);
delayMicroseconds (5) ;
digitalWrite (TRIG_PIN, HIGH);
delayMicroseconds (10) ;
digitalWrite (TRIG_PIN, LOW);
duration = pulseIn (ECHO_PIN, HIGH) ;
cms = (duration/2) / 29.1;
if (isnan(cms))
{
  Serial.println("Failed to get reading from HCRS04");
  return "--";
}
else
{
return String(cms) + " Cm";
}

}

void setup() {
  // put your setup code here, to run once:
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the network");

  client.setServer(mqtt_broker, mqtt_port);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
  // put your main code here, to run repeatedly:
  client.setCallback(callback);
  while (!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.println("Connecting to mqtt server\n");
    if(client.connect(client_id.c_str(),mqtt_username,mqtt_password))
    {
      Serial.println("Connected to MQTT server");
    }
    else
    {
      Serial.println("Failed to connect ");
      Serial.print(client.state());
      delay(1000);
    }
  }
  client.loop(); 

  unsigned long now = millis();
  if (now - previous > stop ) {
    previous = n  ow;

    String dst = getDis();
    client.publish(topic, dst.c_str());
  } 
}
