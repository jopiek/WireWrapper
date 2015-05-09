/*

   TestApp i2c Modules
   
   
*/

#include <i2c_t3.h>
#include <SPI.h>
#include <SdFat.h>
#include <Time.h>
#include <TeensyRTC.h>
#include <ArduinoJson.h>
#include <Vector.h>

#include "WireClass.h"

WireClass          wireTest(I2C_RATE_2400);
const boolean getDebugInfo = true;
unsigned long logTimer = 0;
int logTimeout = 100;
int linkLedSpeed = 0;

/*

Note: following variables are now declared in SDCardControl.h
SdFat sd;
SdFile file;
SdFile dirFile;

ArduinoOutStream cout(Serial);
*/

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
