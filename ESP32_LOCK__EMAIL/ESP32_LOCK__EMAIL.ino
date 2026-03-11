/*
This code is for my IoT project the Smart System Integration for Automobiles
This is run on an ESP32
This one controls a servo motor locking system depending on what rfid tag you use on the rfid reader
It also locks and sends an email depending on the values recieved from the mqtt broker
*/

//Libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include "cred.h"
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

//Pins
#define SS_PIN 5
#define RST_PIN 0

//MQTT config
const char *mqtt_broker = "165.22.122.17";
const char *topic1 = "sensor/alcohol";
const char *topic2 = "gps/address";
const char *topic3 = "sensor/door";
const char *topic4 = "sensor/lock";
const int mqtt_port = 1883;

uint32_t timer = millis();
unsigned long previous = 0;
const long stop = 2000;

//Variables
Servo motor;
MFRC522 mfrc522(SS_PIN, RST_PIN);

enum RFIDMode {
  NORMAL_MODE,
  REGISTER_MODE
};

RFIDMode currentMode = NORMAL_MODE;

String allowedCards[20];
int allowedCardCount = 0;

WiFiClient espClient;
PubSubClient client(espClient);

const int pin = 14;
bool LockCheck = true;
String al, ad, lockStat;

//Called whenever a topic receives data and updates the variable for that topic
void callback(char *topic, byte *payload, unsigned int length) {

  if (strcmp(topic, topic1) == 0) {
    al = "";
    for (int i = 0; i < length; i++) {
      al += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic2) == 0) {
    ad = "";
    for (int i = 0; i < length; i++) {
      ad += ((char)payload[i]);
    }
  }

  if (strcmp(topic, topic4) == 0) {
    lockStat = "";
    for (int i = 0; i < length; i++) {
      lockStat += ((char)payload[i]);
    }
  }
}

String getlk() {
  if (LockCheck) {
    return "Closed";
  } else {
    return "Open";
  }
}

String readCardUID() {
  String cardID = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      cardID += "0";
    }
    cardID += String(mfrc522.uid.uidByte[i], HEX);
  }

  cardID.toUpperCase();
  return cardID;
}

bool isAuthorizedCard(String uid) {
  for (int i = 0; i < allowedCardCount; i++) {
    if (allowedCards[i] == uid) {
      return true;
    }
  }
  return false;
}

bool addNewCard(String uid) {
  if (allowedCardCount >= 20) {
    return false;
  }

  if (isAuthorizedCard(uid)) {
    return false;
  }

  allowedCards[allowedCardCount] = uid;
  allowedCardCount++;
  return true;
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
  allowedCards[0] = "B7C3B001";
  allowedCardCount = 1;

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
      client.subscribe(topic2);
      client.subscribe(topic4);
    } else {
      Serial.println("Failed to connect ");
      Serial.print(client.state());
      delay(1000);
    }
  }
  client.loop();

  //Checks for high alcohol level
  if (al == "Alcohol level: High") {
    String emailBody = al + "\nLocation: " + ad;
    sendMail("Alert", emailBody);  //Sends email
    lock();                        //Locks
    LockCheck = true;
  }

  if (lockStat == "Open") {
    unlock();
    LockCheck = false;
    lockStat = "";
    Serial.println("Unlock Website button\n");
  } else if (lockStat == "Closed") {
    lock();
    LockCheck = true;
    lockStat = "";
    Serial.println("Lock Website button\n");
  }

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "register") {
      currentMode = REGISTER_MODE;
      Serial.println("REGISTER MODE ENABLED - Scan a new card");
    }
  }

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    String cardID = readCardUID();

    Serial.print("Scanned Card UID: ");
    Serial.println(cardID);

    if (currentMode == REGISTER_MODE) {
      if (addNewCard(cardID)) {
        Serial.println("New card registered successfully");
      } else {
        Serial.println("Card already exists or storage full");
      }

      currentMode = NORMAL_MODE;
      Serial.println("Back to NORMAL MODE");
    } else if (currentMode == NORMAL_MODE) {
      if (isAuthorizedCard(cardID)) {
        Serial.println("Access granted");

        if (LockCheck) {
          unlock();
          LockCheck = false;
        } else {
          lock();
          LockCheck = true;
        }
      } else {
        Serial.println("Access denied. Unauthorized card");
        lock();
        LockCheck = true;
      }
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }

  unsigned long now = millis();
  if (now - previous > stop) {
    previous = now;

    String lk = getlk();
    client.publish(topic3, lk.c_str());
  }
}

//Send email using mailgun api
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

//Lock and unlock using servo motor
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