/*
This code is for my IoT project the Smart System Integration for Automobiles
This is run on an ESP32
This one controls a servo motor locking system depending on what rfid tag you use on the rfid reader
It also locks and sends an email depending on the values recieved from the mqtt broker
*/

//Libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
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
const char *topic5 = "sensor/registercard";
const int mqtt_port = 1883;

uint32_t timer = millis();
unsigned long previous = 0;
const long stop = 2000;
unsigned long lastCardSync = 0;
const unsigned long cardSyncInterval = 5000;

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
int pendingUserId = -1;
String pendingCardLabel = "";

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

  if (strcmp(topic, topic5) == 0) {
    String registerPayload = "";
    for (int i = 0; i < length; i++) {
      registerPayload += (char)payload[i];
    }

    Serial.print("Register payload received: ");
    Serial.println(registerPayload);

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, registerPayload);

    if (!error) {
      String mode = doc["mode"] | "";

      if (mode == "register") {
        pendingUserId = doc["user_id"].as<int>();
        pendingCardLabel = doc["card_label"] | "";

        if (pendingUserId <= 0) {
          Serial.println("Invalid pending user ID");
          return;
        }

        currentMode = REGISTER_MODE;

        Serial.println("REGISTER MODE ENABLED");
        Serial.print("Pending user ID: ");
        Serial.println(pendingUserId);
        Serial.print("Pending card label: ");
        Serial.println(pendingCardLabel);
      }
    } else {
      Serial.print("Failed to parse register JSON: ");
      Serial.println(error.c_str());
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

void syncCardsFromServer() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, cannot sync cards");
    return;
  }

  WiFiClientSecure SecureClient;
  SecureClient.setInsecure();

  HTTPClient http;
  http.begin(SecureClient, route);

  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    Serial.println(payload);

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("JSON parse failed: ");
      Serial.println(error.c_str());
      http.end();
      return;
    }

    allowedCardCount = 0;

    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
      if (allowedCardCount < 20) {
        allowedCards[allowedCardCount] = obj["card_uid"].as<String>();
        allowedCards[allowedCardCount].toUpperCase();
        allowedCardCount++;
      }
    }

    Serial.print("Loaded cards: ");
    Serial.println(allowedCardCount);
  } else {
    Serial.print("HTTP GET failed, code: ");
    Serial.println(httpCode);
  }

  http.end();
}

bool registerCardToServer(String cardUID, int userID, String cardLabel) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, cannot register card");
    return false;
  }

  WiFiClientSecure secureClient;
  secureClient.setInsecure();

  HTTPClient http;
  http.begin(secureClient, registerRoute);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(512);
  doc["card_uid"] = cardUID;
  doc["user_id"] = userID;
  doc["card_label"] = cardLabel;

  String jsonBody;
  serializeJson(doc, jsonBody);

  int httpCode = http.POST(jsonBody);

  Serial.print("Register card HTTP code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
    http.end();
    return (httpCode == 200 || httpCode == 201);
  }

  Serial.println("HTTP POST failed");
  http.end();
  return false;
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

  syncCardsFromServer();
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
      client.subscribe(topic5);
    } else {
      Serial.println("Failed to connect ");
      Serial.print(client.state());
      delay(1000);
    }
  }
  client.loop();

  if (millis() - lastCardSync > cardSyncInterval) {
    lastCardSync = millis();
    syncCardsFromServer();
  }

  //Checks for high alcohol level
  if (al == "High") {
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
      if (pendingUserId == -1) {
        Serial.println("No pending user ID set. Cannot register card.");
      } else {
        bool success = registerCardToServer(cardID, pendingUserId, pendingCardLabel);

        if (success) {
          Serial.println("New card registered successfully in database");
          syncCardsFromServer();
        } else {
          Serial.println("Failed to register card in database");
        }
      }

      pendingUserId = -1;
      pendingCardLabel = "";
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