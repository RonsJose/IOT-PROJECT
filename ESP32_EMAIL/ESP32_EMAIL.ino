#include <WiFi.h>
#include <HTTPClient.h>
#include "cred.h"

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected!");

  sendMail("Hello", "Test");
}

void loop() {}

void sendMail(String subject, String body) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "https://api.mailgun.net/v3/";
  url += mailgunDomain;
  url += "/messages";

  http.begin(url);
  http.setAuthorization("api", mailgunAPIKey);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postData = "from=ESP32 <mail@" + String(mailgunDomain) + ">";
  postData += "&to=" + String(recipientEmail);
  postData += "&subject=" + subject;
  postData += "&text=" + body;

  int httpCode = http.POST(postData);
  Serial.println(httpCode);

  if (httpCode > 0) {
    Serial.println(http.getString());
  } else {
    Serial.println("Error sending email");
  }

  http.end();
}