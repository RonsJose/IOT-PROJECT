#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

#define SS_PIN 5
#define RST_PIN 0

Servo motor;
MFRC522 mfrc522(SS_PIN, RST_PIN);

const int pin = 14;
bool LockCheck = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Hold the card close to the reader");
  Serial.println();
  motor.setPeriodHertz(50);
  motor.attach(pin, 500, 2400);
  motor.write(0);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
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
      }
    else {
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