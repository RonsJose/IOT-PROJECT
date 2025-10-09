#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "webpagecode.h"
#include "WifiCred.h"
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 27
const int TRIG_PIN = 25;
const int ECHO_PIN = 14;
long duration;
float cms, inches;

WebServer server(80);

String getTemp() {
   DHT.read(DHT11_PIN);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  return String(DHT.temperature);
}

String getHumid(){
   DHT.read(DHT11_PIN);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  return String(DHT.humidity);
}

String getDis(){
digitalWrite (TRIG_PIN, LOW);
delayMicroseconds (5) ;
digitalWrite (TRIG_PIN, HIGH);
delayMicroseconds (10) ;
digitalWrite (TRIG_PIN, LOW);
duration = pulseIn (ECHO_PIN, HIGH) ;
cms = (duration/2) / 29.1;
return String(cms);
}

void handleRoot() {
  String message = homePagePart1 + getTemp() + homePagePart2 + getHumid() + homePagePart3 + getDis() + homePagePart4;
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
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
  
  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}
