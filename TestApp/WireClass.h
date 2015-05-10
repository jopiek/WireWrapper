/*
WireClass
9 mei 2015

Wrapper Around i2c_t3
Used in RobotPatient.com Simulators
*/

#pragma once

#ifndef WireClass_h
#define WireClass_h
#include "Arduino.h"
#include "i2c_t3.h" // we assume i2c_t3.h is included
#include "MyModule.h"


// definitions of global items

// i2c commands from Master:
#define WRITE       			0x10
#define READ        			0x20
#define READWORD     			0x22
#define READLONG     			0x24
#define SETRATE     			0x30

#define START_ACTION   			0x40
#define START_ACTION_DUMP   0x50
#define STOP_ACTION     		0x60

#define CPR_MODULE_ADDR 	  0x42
#define CPR_BUFLEN  			  0x30 			// 3*16 = 48 decimaal ruimte
#define I2C_BUS 						0				  // default BUS 0

/*
// Addresses of Simulation Add-ons
const uint8_t Main_Controller_Address 		= 0x40; // slave addr
const uint8_t ECG_Controller_Address  	  = 0x42; // slave addr
const uint8_t CPR_Controller_Address  	  = 0x44; // slave addr
const uint8_t Touch_Controller_Address    = 0x46; // slave addr, reserved
const uint8_t AUSC_Controller_Address     = 0x48; // slave addr, reserved
const uint8_t HEAD_Controller_Address     = 0x50; // slave addr, reserved

*/


/*
 CPRControl States

 Note 1: Wire can only politely ask and stores it in RequestedCPRState
 CPRControl is responsible: sets the state by asking for getRequestedCPRState

 Note 2: These states are compatible with CPRStates


*/

// Addresses below are for the CPRModule

// --------  Compressions -------- //

const byte cpmMemAddr	 				  = 0x00; //  compressions per minute (word / cpm)
const byte cDepth0MemAddr	 		  = 0x02; //  compression depth highest (relaxed) point (word / mm)
const byte cTimeStamp0MemAddr	  = 0x04; //  compression timestamp highest point (ulong millis)
const byte cDepth1MemAddr	 		  = 0x08; //  compression depth lowest (compressed) point (word / mm)
const byte cTimeStamp1MemAddr	  = 0x0A; //  compression timestamp lowest point (ulong millis)
const byte cCountMemAddr	 	    = 0x0E; //  compression cumulative counter for session (#)

// --------  Ventilations -------- //

const byte vpmMemAddr	 				  = 0x10; //  ventilations per minute (word / cpm)
const byte vVolumeMemAddr	 		  = 0x12; //  ventilation inFlow volume (cc)
const byte vTimeStampMemAddr	  = 0x14; //  ventilation timestamp inFlow (ulong millis)
const byte vCumVolumeMemAddr	  = 0x18; //  ventilation depth highest (relaxed) point (word / mm)
const byte vCumTimeStampMemAddr = 0x1A; //  ventilation cumulative volume timestamp expired (ulong millis)
const byte vCountMemAddr	 	    = 0x1E; //  ventilation cumulative counter for session (#)

// ------  Finger placement ------ //

const byte fFinger0MemAddr		  = 0x22; //  finger sensor 0 value (word / pressure)
const byte fTimeStamp0MemAddr	  = 0x24; //  finger sensor 0 timestamp (ulong millis)
const byte fFinger1MemAddr	 	  = 0x28; //  finger sensor 1 value (word / pressure)
const byte fTimeStamp1MemAddr	  = 0x2A; //  finger sensor 1 timestamp (ulong millis)

// ------------------------------- //

const int numModules = 7;


class WireClass
{
  public:
    WireClass(i2c_rate speed);

    MyModule* myModules = new MyModule[numModules];

    void initWire();
    void initModules();

    int initTWIModule(uint8_t module_address);

    void stopAction();
    void startAction();
    byte getModuleIdentifiers();

    void printModules();


  private:

    int print_i2c_status();

    word readWordValueFromWire(byte memaddr, byte slave_addr);
    unsigned long readLongValueFromWire(byte memaddr, byte slave_addr);
    byte readByteValueFromWire(byte memaddr, byte slave_addr);
    
    boolean moduleCorrectAndAvailable(SimModule module, byte id);


    static uint8_t cprValBuffer[CPR_BUFLEN];
    static i2c_rate i2c_speed;


    void writeWordValueToMemory(word value, byte memaddr);
    void writeULongValueToMemory(unsigned long value, byte memaddr);
    void writeByteValueToMemory(byte value, byte memaddr);


    static i2c_rate i2c_master_speed;



};

#endif

/*

// -------------------------------------------------------------------------------------------
// Teensy3.0/3.1 I2C Interrupt Test
// 15May13 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// This creates an I2C master device which will issue I2C commands from inside a periodic
// interrupt (eg. reading a sensor at regular time intervals).  For this test the Slave device
// will be assumed to be that given in the i2c_t3/slave sketch.
//
// The test will start when the Serial monitor opens.
//
// This example code is in the public domain.
//
// -------------------------------------------------------------------------------------------
// Slave protocol is as follows:
// -------------------------------------------------------------------------------------------
// WRITE - The I2C Master can write to the device by transmitting the WRITE command,
//         a memory address to store to, and a sequence of data to store.
//         The command sequence is:
//
// START|I2CADDR+W|WRITE|MEMADDR|DATA0|DATA1|DATA2|...|STOP
//
// where START     = I2C START sequence
//       I2CADDR+W = I2C Slave address + I2C write flag
//       WRITE     = WRITE command
//       MEMADDR   = memory address to store data to
//       DATAx     = data byte to store, multiple bytes are stored to increasing address
//       STOP      = I2C STOP sequence
// -------------------------------------------------------------------------------------------
// READ - The I2C Master can read data from the device by transmitting the READ command,
//        a memory address to read from, and then issuing a STOP/START or Repeated-START,
//        followed by reading the data.  The command sequence is:
//
// START|I2CADDR+W|READ|MEMADDR|REPSTART|I2CADDR+R|DATA0|DATA1|DATA2|...|STOP
//
// where START     = I2C START sequence
//       I2CADDR+W = I2C Slave address + I2C write flag
//       READ      = READ command
//       MEMADDR   = memory address to read data from
//       REPSTART  = I2C Repeated-START sequence (or STOP/START if single Master)
//       I2CADDR+R = I2C Slave address + I2C read flag
//       DATAx     = data byte read by Master, multiple bytes are read from increasing address
//       STOP      = I2C STOP sequence
// -------------------------------------------------------------------------------------------
// RobotPatient uitbreidingen feb 2015
// -------------------------------------------------------------------------------------------
// Voor normale mode.
// [compression value: 2 bytes][compressions per minute: 2 bytes][ventilations value: 2 bytes]
// [ventilations per minute: 2 bytes][fingers place 1: 1 byte][fingers place 2: 1 byte][timestamp : 4 bytes]
// [0,1][2,3][4,5][6,7][8,9][10,11][12,13,14,15]
// Dump mode nog implementeren, even goed denken hoe we dat doen,
// dan iets van elke tussenmeting ook oversturen oid.
// Is wel nuttig / belangrijk voor expert reanimaties vergelijken.
// -------------------------------------------------------------------------------------------

#include <i2c_t3.h>

// Command definitions
#define WRITE 0x10
#define READ  0x20
#define READWORD     0x22
#define READLONG     0x24

// Function prototypes
void readSlave(void);


#define START_CPR     	  0x40
#define START_CPR_DUMP    0x50
#define STOP_CPR	  0x60


// CPRControl States
#define CPRStateIdle 	    0
#define CPRStateActive 	    1
#define CPRStateActiveDump  2


int CPRState = CPRStateIdle;

/*
void setup()
{

  Serial.begin(115200);
  delay(1000);

  pinMode(debug_led,OUTPUT);        // LED
  digitalWrite(debug_led,HIGH);        // LED

  // Setup for Master mode, pins 18/19, external pullups, 400kHz
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_2400);
  digitalWrite(debug_led,LOW);        // LED

  Serial.println("CPR DATA:");
  delay(500);
  startCPR();
  delay(5000);
  startCPR();
  digitalWrite(debug_led,HIGH);         // pulse LED when reading
}* /

/*
void loop()
{
  //digitalWrite(debug_led,HIGH);         // pulse LED when reading
  //getCTimeStamp1();
  //printVentilationStuff();
  printCompressionStuff();
}* /

void WireClass::stopCPR() {
  Wire.beginTransmission(target);         // slave addr
  Wire.write(STOP_CPR);                       // WRITE command
  Wire.endTransmission(I2C_NOSTOP);       // blocking Tx, no STOP
}

void startCPR() {
  Wire.beginTransmission(target);         // slave addr
  Wire.write(START_CPR);                       // WRITE command
  Wire.endTransmission(I2C_NOSTOP);       // blocking Tx, no STOP
}

word getCPM() {
  return readWordFromWireAddr(0x0);
}

unsigned long getcTimeStamp1() {
  return readLongFromWireAddr(0x4);
}

word getVPM() {
  return readWordFromWireAddr(0x10);
}

unsigned long getvTimeStamp() {
  return readLongFromWireAddr(0x14);
}

void WireClass::printVentilationStuff() {
  Serial.print(readWordFromWireAddr(0x10));
  Serial.print(", ");
  Serial.print(readWordFromWireAddr(0x12)/10);
  Serial.print(", ");
  Serial.print(readLongFromWireAddr(0x14));
  Serial.print(", ");
  Serial.print(readWordFromWireAddr(0x18));
  Serial.print(", ");
  Serial.print(readLongFromWireAddr(0x1A));
  Serial.print(", ");
  Serial.print(readWordFromWireAddr(0x1E)/10);
  Serial.println(";");
}

void WireClass::printCompressionStuff() {
  Serial.print(readWordFromWireAddr(0x0));
  Serial.print(", ");
  Serial.print(readWordFromWireAddr(0x2));
  Serial.print(", ");
  Serial.print(readLongFromWireAddr(0x4));
  Serial.print(", ");
  Serial.print(readWordFromWireAddr(0x8));
  Serial.print(", ");
  Serial.print(readLongFromWireAddr(0xA));
  Serial.print(", ");
  Serial.print(readWordFromWireAddr(0xE));
  Serial.println(";");
}

void WireClass::printCPRData(unsigned long currtimestamp) {
  for (int counter = 0; counter < 5; counter++) {
    Serial.print(CPRItems[counter]);
    Serial.print(",");
    // later direct naar CSV op SD kaart
  }
  Serial.println(currtimestamp);
  Serial.print("Own Timestamp: ");
  Serial.println(millis());
}

word WireClass::readWordFromWireAddr(byte _address) {
  Wire.beginTransmission(target);         // slave addr
  Wire.write(READWORD);                   // command
  Wire.write(_address);                        // memory address
  Wire.endTransmission(I2C_NOSTOP);       // blocking Tx, no STOP
  Wire.requestFrom(target,2,I2C_STOP);    // READ 1 byte
  byte dataIn1 = Wire.readByte();
  byte dataIn2 = Wire.readByte();
  word myOutput = ((dataIn1 << 8) | dataIn2);
  return myOutput;
}

byte WireClass::readByteFromWireAddr(byte _address) {
  Wire.beginTransmission(target);         // slave addr
  Wire.write(READ);                   // command
  Wire.write(_address);                        // memory address
  Wire.endTransmission(I2C_NOSTOP);       // blocking Tx, no STOP
  Wire.requestFrom(target,2,I2C_STOP);    // READ 1 byte
  byte dataIn1 = Wire.readByte();
  return dataIn1;
}

unsigned long WireClass::readLongFromWireAddr(byte _address) {
  Wire.beginTransmission(target);         // slave addr
  Wire.write(READLONG);                   // command
  Wire.write(_address);                        // memory address
  Wire.endTransmission(I2C_NOSTOP);       // blocking Tx, no STOP
  Wire.requestFrom(target,4,I2C_STOP);    // READ 1 byte
  byte dataIn1 = Wire.readByte();
  byte dataIn2 = Wire.readByte();
  byte dataIn3 = Wire.readByte();
  byte dataIn4 = Wire.readByte();
  unsigned long myOutput = ((dataIn1 << 32) | (dataIn2 << 16) | (dataIn3 << 8) | dataIn4);
  return myOutput;
}

*/
