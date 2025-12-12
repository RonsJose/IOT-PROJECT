#include <WiFi.h>
#include <HTTPClient.h>
#include "cred.h"
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

#define SS_PIN 5
#define RST_PIN 0

const char *mqtt_broker = "165.22.122.17";
const char *topic1 = "sensor/alcohol";
const int mqtt_port = 1883;

Servo motor;
MFRC522 mfrc522(SS_PIN, RST_PIN);

WiFiClient espClient;
PubSubClient client(espClient);

const int pin = 14;
bool LockCheck = true;
String al;

void callback(char *topic, byte *payload, unsigned int length) {

  if (strcmp(topic, topic1) == 0) {
    al = "";
    for (int i = 0; i < length; i++) {
      al += ((char)payload[i]);
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected!");

  SPI.begin();
  mfrc522.PCD_Init();
  motor.setPeriodHertz(50);
  motor.attach(pin, 500, 2400);
  motor.write(0);

  client.setCallback(callback);
  client.setServer(mqtt_broker, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.println("Connecting to mqtt server\n");
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT server");
      client.subscribe(topic1);
    } else {
      Serial.println("Failed to connect ");
      Serial.print(client.state());
      delay(1000);
    }
  }
  client.loop();

  if (al == "Alcohol level: High") {
    sendMail("Alert", al);
  }

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card ID: ");
  String cardID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    cardID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    cardID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();

  cardID.toUpperCase();
  if (cardID.substring(1) == "B7 C3 B0 01") {
    Serial.println("Access granted");
    Serial.println();
    if (LockCheck) {
      unlock();
      LockCheck = false;
    } else {
      lock();
      LockCheck = true;
    }
    delay(2000);
  } else {
    Serial.println("Access denied. Unauthorized card");
    lock();
    LockCheck = true;
    delay(2000);
  }
}

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

void unlock() {
  Serial.println("Unlocking");
  motor.write(90);
  delay(500);
}

void lock() {
  Serial.println("Locking");
  motor.write(0);
  delay(500);
}