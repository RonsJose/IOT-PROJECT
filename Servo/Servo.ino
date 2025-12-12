#include <ESP32Servo.h>

Servo lock;

const int pin = 14; 

void setup() {
  Serial.begin(115200);

 lock.setPeriodHertz(50);   
 lock.attach(pin, 500, 2400);

 lock.write(0);
  delay(500);

  Serial.println("Type 'u' to unlock, 'l' to lock");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    if (c == 'u') {
      unlock();
    } else if (c == 'l') {
      lock();
    }
  }
}

void unlock() {
  Serial.println("Unlocking");
 lock.write(90);
  delay(500);
}

void lock() {
  Serial.println("Locking");
 lock.write(0);
  delay(500);
}