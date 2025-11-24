#include <WiFi.h>
#include <NetworkClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "webpagecode.h"
#include <PubSubClient.h>
#include "cred.h"

AsyncWebServer server(80);

WiFiClient espClient;
PubSubClient client(espClient);

String Temp, Dist, Humidity, Blood, Heart;

const char *mqtt_broker = "165.22.122.17";
const char *topic1 = "sensor/distance";
const char *topic2 = "sensor/temperature";
const char *topic3 = "sensor/humidity";
const char *topic4 = "sensor/heartrate";
const char *topic5 = "sensor/blood";
const int mqtt_port = 1883;

void handleRoot(AsyncWebServerRequest *request) {
  String page = String(homePage);
  page.replace("%TEMPERATURE%", Temp.c_str());
  page.replace("%HUMIDITY%", Humidity.c_str());
  page.replace("%DISTANCE%", Dist.c_str());
  page.replace("%HEARTRATE%", Heart.c_str());
  page.replace("%SPO2%", Blood.c_str());
  request->send(200, "text/html", page);
}

void handleNotFound(AsyncWebServerRequest *request) {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->params();
  message += "\n";
  for (uint8_t i = 0; i < request->params(); i++) {
    const AsyncWebParameter *p = request->getParam(i);
    message += " " + p->name() + ": " + p->value() + "\n";
  }
  request->send(404, "text/plain", message);
}

void callback(char *topic, byte *payload, unsigned int length) {

  if (strcmp(topic, topic1) == 0) {
    Dist = "";
    for (int i = 0; i < length; i++) {
      Dist += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic2) == 0) {
    Temp = "";
    for (int i = 0; i < length; i++) {
      Temp += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic3) == 0) {
    Humidity = "";
    for (int i = 0; i < length; i++) {
      Humidity += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic4) == 0) {
    Heart = "";
    for (int i = 0; i < length; i++) {
      Heart += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic5) == 0) {
    Blood = "";
    for (int i = 0; i < length; i++) {
      Blood += ((char)payload[i]);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("iot")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", HTTP_GET, handleRoot);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", Temp.c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", Humidity.c_str());
  });

  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", Dist.c_str());
  });

  server.on("/heartRate", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", Heart.c_str());
  });
  server.on("/spO2", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", Blood.c_str());
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

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

      client.subscribe(topic1);
      client.subscribe(topic2);
      client.subscribe(topic3);
      client.subscribe(topic4);
      client.subscribe(topic5);

      client.setCallback(callback);
    } else {
      Serial.println("Failed to connect ");
      Serial.print(client.state());
      delay(1000);
    }
  }
  client.loop();
}
