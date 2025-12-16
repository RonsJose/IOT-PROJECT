/*
This code is for my IoT project the Smart System Integration for Automobiles
This is run on an ESP32
This one is responsible for the webserver and uploading data to thingspeak
It hosts the async webserver and sends the sensor data to thingspeak 
*/

//Libraries
#include <WiFi.h>
#include <NetworkClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "webpagecode.h"
#include <PubSubClient.h>
#include "ThingSpeak.h"
#include "cred.h"

//Webserver port
AsyncWebServer server(80);

//Thingspeak Config
unsigned long channelNum = 3151130;
#define UPLOAD_PERIOD 20000
unsigned long uploadLast = 0;#

//MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

//Sensor data
String Temp, Dist, Humidity, Blood, Heart, Long, Lat, ip, alcohol, address;

//MQTT config
const char *mqtt_broker = "165.22.122.17";
const char *topic1 = "sensor/distance";
const char *topic2 = "sensor/temperature";
const char *topic3 = "sensor/humidity";
const char *topic4 = "sensor/heartrate";
const char *topic5 = "sensor/blood";
const char *topic6 = "sensor/latitude";
const char *topic7 = "sensor/longitude";
const char *topic8 = "camera/ip";
const char *topic9 = "sensor/alcohol";
const char *topic10 = "gps/address";
const int mqtt_port = 1883;

//Loads template and replaces placeholder values 
void handleRoot(AsyncWebServerRequest *request) {
  String page = String(homePage);
  page.replace("%TEMPERATURE%", Temp.c_str());
  page.replace("%HUMIDITY%", Humidity.c_str());
  page.replace("%DISTANCE%", Dist.c_str());
  page.replace("%HEARTRATE%", Heart.c_str());
  page.replace("%SPO2%", Blood.c_str());
  page.replace("%IP%", ip.c_str());
  page.replace("%ALCOHOL%", alcohol.c_str());
  page.replace("%ADDRESS%",address.c_str());
  request->send(200, "text/html", page);
}

//Handles errors
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

//Called whenever a topic receives data and updates the variable for that topic
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

  if (strcmp(topic, topic6) == 0) {
    Lat = "";
    for (int i = 0; i < length; i++) {
      Lat += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic7) == 0) {
    Long = "";
    for (int i = 0; i < length; i++) {
      Long += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic8) == 0) {
    ip = "";
    for (int i = 0; i < length; i++) {
      ip += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic9) == 0) {
    alcohol = "";
    for (int i = 0; i < length; i++) {
      alcohol += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic10) == 0) {
    address = "";
    for (int i = 0; i < length; i++) {
      address += ((char)payload[i]);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  //Connect to wifi, start serial, starts thingspeak client, configs mqtt client and callback, and configs the HTTP routes
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(espClient);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //MDNS for when at home for wifi
  if (MDNS.begin("iot")) {
    Serial.println("MDNS responder started");
  }

  //Loads the main webpage
  server.on("/", HTTP_GET, handleRoot);

  //HTTP GET endpoints that send back the current value of whatever sensor that is requested
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

  server.on("/latitude", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", Lat.c_str());
  });

  server.on("/longitude", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", Long.c_str());
  });

  server.on("/camera", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", ip.c_str());
  });

  server.on("/alcohol", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", alcohol.c_str());
  });

  server.on("/address", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", address.c_str());
  });

  //Handles server not working
  server.onNotFound(handleNotFound);

  //starts server
  server.begin();
  Serial.println("HTTP server started");

  //mqtt broker and callback config
  client.setCallback(callback);
  client.setServer(mqtt_broker, mqtt_port);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Connects to mqtt broker and subscribes to all the topics 
  if (!client.connected()) {
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
      client.subscribe(topic6);
      client.subscribe(topic7);
      client.subscribe(topic8);
      client.subscribe(topic9);
      client.subscribe(topic10);
    } else {
      Serial.println("Failed to connect ");
      Serial.print(client.state());
      delay(1000);
    }
  }
  client.loop();

  //Uploads data to thingspeak every 20 seconds
  unsigned long now = millis();
  if (now - uploadLast >= UPLOAD_PERIOD) {

    if (Temp == "" || Humidity == "" || Dist == "" || Heart == "" || Blood == "") { //Skips uploading if one of the variables is empty to avoid errors
      Serial.println("skip");
      uploadLast = now;
      return;
    }

    ThingSpeak.setField(1, Temp.c_str());
    ThingSpeak.setField(2, Humidity.c_str());
    ThingSpeak.setField(3, Heart.c_str());
    ThingSpeak.setField(4, Blood.c_str());
    ThingSpeak.setField(5, Dist.c_str());

    int result = ThingSpeak.writeFields(channelNum, thingspeak);
    Serial.println(result);

    uploadLast = now;
  }
}
