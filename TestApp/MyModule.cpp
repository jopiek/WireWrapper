/*
 SimControl Module Library: SimModue
 Jannes Bloemert / Johan Korten
 February 2015
 
 RobotPatient.com Development BV 
 
 Eenvoudige module om modules OO te kunnen bijhouden.
 
 Modules hebben:
 - naam: leesbare naam (char / string)
 - id: hexadecimaal adres (wire/i2c)
 - available: false / true
 
 Na detectie op de bus wordt available true, standaard false.
 
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
