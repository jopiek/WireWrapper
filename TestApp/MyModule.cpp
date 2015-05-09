/*
 MyModule Module Library: MyModule
 Jannes Bloemert / Johan Korten
 Original: February 2015

 RobotPatient.com Development BV
*/

#pragma once
#include "MyModule.h"
#include "Arduino.h"

#ifndef MyModule_cpp
#define MyModule_cpp

MyModule::MyModule() {
  // default constructor
  id = 0;
  name = "";
  available = false;
}

MyModule::MyModule(byte _id, char *_name) {
  id = _id;
  //strcpy(name, _name);
  name = _name;
  available = false;
}

void MyModule::print() {
  Serial.print("ID: ");
  Serial.print(id, HEX);
  Serial.print(" name: ");
  Serial.print(name);
  Serial.print(" available: ");
  Serial.print(available);
}

void MyModule::printName() {
  Serial.println(name);
}


#endif
