/*

   TestApp i2c Modules
   9th of May 2015

*/

#include <i2c_t3.h>
#include "WireClass.h"

WireClass          wireTest(I2C_RATE_2400);
const boolean getDebugInfo = true;
unsigned long logTimer = 0;
int logTimeout = 100;
int linkLedSpeed = 0;

int heartbeatLED = 13;
unsigned long hbTimer = 0;
boolean hbLEDState = false;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("TestApp version 1.0");
  Serial.println("System starting...");
  wireTest.initWire();
  delay(1000);
  wireTest.initModules();
  byte modules = wireTest.getModuleIdentifiers();
  pinMode(heartbeatLED, OUTPUT);
}

void loop() {
  checkSerial();
  heartbeat();
}


void checkSerial() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 'b') {
      Serial.println("Start Action");
      wireTest.startAction();
    } else if (input == 'e') {
      Serial.println("Stop Action");
      wireTest.stopAction();
    }  else if (input == 'p') {
      Serial.println("Print all modules...");
      wireTest.printModules();
    }

  }
}

void heartbeat() {
  if ((millis() - hbTimer > 100 && hbLEDState) || (millis() - hbTimer > 250)) {
    hbTimer = millis();
    hbLEDState = !hbLEDState;
    digitalWrite(heartbeatLED, hbLEDState);
  }
}
