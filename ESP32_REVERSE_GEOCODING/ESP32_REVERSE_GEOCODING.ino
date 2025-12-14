#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "cred.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) delay(500);

  HTTPClient http;
  String link = "https://maps.googleapis.com/maps/api/geocode/json?";
  link += "latlng=";
  link += String(53.267471197914794,6);
  link += ",";
  link += String(-8.922346348122888,6);
  link += "&key=";
  link += google_api;
  http.begin(link);

  int httpCode = http.GET();
  if (httpCode > 0) {
    String json = http.getString();
    JSONVar jsonVar = JSON.parse json);

    if (JSON.typeof jsonVar["results"]) == "array" && jsonVar["results"].length() > 0 && JSON.typeof jsonVar["results"][0]["formatted_address"]) == "string") {

      Serial.println((const char* jsonVar["results"][0]["formatted_address"]);

    } else {
      Serial.println("No addresses");
    }
  }

  http.end();
}

void loop() {}
