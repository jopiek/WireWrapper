/*
WireClass
9 mei 2015

Wrapper Around i2c_t3
Used in RobotPatient.com Simulators


*/

#ifndef WireClass_cpp
#define WireClass_cpp

#include "Arduino.h"
#include "WireClass.h"
#include "MyModule.h"

#include <i2c_t3.h>

boolean debug_serial = !false;

IntervalTimer slaveTimer;
//uint8_t target = 0x42; //0x44;
size_t addr = 0, len;

int debug_led = 14;
unsigned long myData = 0;
unsigned long prevTimeStamp = -1;
word CPRItems[6] = {
  0, 0, 0, 0, 0, 0
};

boolean finished = false;

/*
  static object variables that need to be redeclared in the cpp:
 */

uint8_t WireClass::cprValBuffer[CPR_BUFLEN];
i2c_rate WireClass::i2c_master_speed;


/*
		constructor and other methods
 */


WireClass::WireClass(i2c_rate speed)
{
  i2c_master_speed = speed;

  MyModule mainModule  = MyModule(0x40, "Module 1");
  myModules[0] = mainModule;

  MyModule module2   = MyModule(0x42, "Module 2");
  myModules[1] = module2;

  MyModule module3   = MyModule(0x44, "Module 3");
  myModules[2] = module3;

  MyModule module4 = MyModule(0x46, "Module 4");
  myModules[3] = module4;

  MyModule module5  = MyModule(0x48, "Module 5");
  myModules[4] = module5;

  MyModule module6  = MyModule(0x50, "Module 6");
  myModules[5] = module6;

  MyModule module7 = MyModule(0x52, "Module 7");
  myModules[6] = module7;

}

void WireClass::initWire() {
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, i2c_master_speed);
}

void WireClass::initModules() {

  for (int i = 0; i <= 6; ++i) {

    if ((myModules[i].name != "SimMainboard")) { // skip myself ;)
      byte i2c_result = initTWIModule(myModules[i].id);
      boolean module_found = false;
      MyModule module = myModules[i];
      Serial.print("Address: ");
      Serial.println(myModules[2].id);
      if (i2c_result == 0) {
        module_found = true;
        if (debug_serial) {
          //Serial.print("Module: ");
          //module.print();
          //Serial.println(" detected...");
          Serial.println(module.id);
          if (module.id == 0x44) {
            Serial.println("CPR Module geïdentificeerd...");
          }
        }
      }
      myModules[i].available = module_found;
    }
  }

}


int WireClass::initTWIModule(uint8_t module_address) {
  Wire.beginTransmission(module_address);
  Wire.endTransmission(I2C_NOSTOP, 500); // ok?
  return print_i2c_status();
}

int WireClass::print_i2c_status(void)
/*
  Post: 0 is OK
 1 is Slave addr not ack
 2 is Slave data not ack
 3 is Bus error
 4 is I2C busy
 -1 is error state
 */
{
  // return: I2C_WAITING, I2C_SENDING, I2C_SEND_ADDR, I2C_RECEIVING, I2C_TIMEOUT, I2C_ADDR_NAK, I2C_DATA_NAK, I2C_ARB_LOST, I2C_SLAVE_TX, I2C_SLAVE_RX

  boolean writeSerialTWI = false;
  //if (writeSerialTWI) I2C_DEBUG_WAIT; // wait for Serial to clear debug msgs (only needed if using debug)
  switch (Wire.status())
  {
    case I2C_WAITING:
      if (writeSerialTWI) Serial.print("I2C waiting, no errors\n");
      return 0;
    case I2C_ADDR_NAK:
      if (writeSerialTWI) Serial.print("Slave addr not acknowledged\n");
      return 1;
    case I2C_DATA_NAK:
      if (writeSerialTWI) Serial.print("Slave data not acknowledged\n");
      return 2;
    case I2C_ARB_LOST:
      if (writeSerialTWI) Serial.print("Bus Error: Arbitration Lost\n");
      return 3;
    default:
      if (writeSerialTWI) Serial.print("I2C busy\n");
      return 4;
  }
  return -1; // should never happen :)
}

void WireClass::stopAction() {

  if ((myModules[2].id == 0x44) || (!myModules[2].available)) {
    Serial.println("Whoops! Can't find my module!!!");
    return;
  }
  byte My_Controller_Address = 0x44; //myModules[2].id;


  Serial.print("Addr: ");
  Serial.print(My_Controller_Address, HEX);
  Serial.print(" cmd: ");
  Serial.print(STOP_ACTION, HEX);
  Serial.print(" stop: ");
  Serial.println(I2C_NOSTOP);

  Wire.beginTransmission(My_Controller_Address);         // slave addr
  Wire.write(STOP_ACTION);                       // WRITE command
  Wire.endTransmission(I2C_NOSTOP);       // blocking Tx, no STOP

}

void WireClass::startAction() {
  // reset optimization values!
  /*
  if ((myModules[2].id == 0x44) || (!myModules[2].available)) {
  	Serial.println("Whoops! Can't find CPR module!!!");
  	return;
  }*/
  byte My_Controller_Address = myModules[2].id;

  Serial.print("Name: ");
  Serial.println(myModules[2].name);
  Serial.print("ID: ");
  Serial.println(myModules[2].id, HEX);

  // Send message to My Module
  Wire.beginTransmission(My_Controller_Address);         // slave addr
  Wire.write(START_ACTION);                       // WRITE command
  Wire.endTransmission(I2C_NOSTOP);       // blocking Tx, no STOP
}



word WireClass::readWordValueFromWire(byte memaddr, byte slave_addr) {

  word result = 0;
  Wire.beginTransmission(slave_addr);         // slave addr
  Wire.write(READWORD);                       // READWORD command
  Wire.write(memaddr);                        // memory address
  Wire.endTransmission(I2C_NOSTOP);       	  // blocking Tx, no STOP
  if (Wire.requestFrom(slave_addr, 2, I2C_STOP) == 2) {    	 // READ 2 bytes
    result = Wire.readByte();
    result = result << 8 | Wire.readByte();
  }
  //Serial.print("Reading Word: ");
  //Serial.println(result);

  return result;
}

unsigned long WireClass::readLongValueFromWire(byte memaddr, byte slave_addr) {
  unsigned long result = 0;
  //Serial.print("Reading Long: ");
  Wire.beginTransmission(slave_addr);         // slave addr
  Wire.write(READLONG);                       // READWORD command
  Wire.write(memaddr);                        // memory address
  Wire.endTransmission(I2C_NOSTOP);       	  // blocking Tx, no STOP
  if (Wire.requestFrom(slave_addr, 4, I2C_STOP) == 4) {    	 // READ 4 bytes
    result = Wire.readByte();
    result = result << 8 | Wire.readByte();
    result = result << 8 | Wire.readByte();
    result = result << 8 | Wire.readByte();
  }
  //Serial.println(result);
  return result;
}

byte WireClass::readByteValueFromWire(byte memaddr, byte slave_addr) {
  byte result = 0;
  Wire.beginTransmission(slave_addr);         // slave addr
  Wire.write(READ);                       	  // READWORD command
  Wire.write(memaddr);                        // memory address
  Wire.endTransmission(I2C_NOSTOP);       	  // blocking Tx, no STOP
  if (Wire.requestFrom(slave_addr, 1, I2C_STOP) == 1) {    	 // READ 1 byte
    return Wire.readByte();
  }
  return result;
}

byte WireClass::getModuleIdentifiers() {

  byte modules = 0;

  for (int i = 1; i < numModules; ++i) {
    if ((myModules[i].id == 0x44) && (myModules[i].available)) modules = modules | 0b1;
    if ((myModules[i].id == 0x42) && (myModules[i].available)) modules = modules | 0b10;
    if ((myModules[i].id == 0x50) && (myModules[i].available)) modules = modules | 0b100;
    if ((myModules[i].id == 0x48) && (myModules[i].available)) modules = modules | 0b1000;
    if ((myModules[i].id == 0x46) && (myModules[i].available))  modules = modules | 0b10000;
    if ((myModules[i].id == 0x52) && (myModules[i].available))  modules = modules | 0b100000;
    Serial.println(modules);
  }
  return modules; // let op, in SimCommunication wordt MSB op één gezet!
}

//===================================================//



// ----------------- Helper Functions ----------------- //

void WireClass::writeWordValueToMemory(word value, byte memaddr) {
  byte highByte = (byte)((value & 0xFF00) >> 8);
  byte lowByte = (byte)(value & 0xFF);
  cprValBuffer[memaddr] = highByte;
  cprValBuffer[memaddr + 1] = lowByte;

  Serial.print("Value: ");
  Serial.print(value);
  Serial.print(" Wrote: ");
  Serial.print(highByte);
  Serial.print(" | ");
  Serial.print(lowByte);
  Serial.print(" to memory at: ");
  Serial.println(memaddr);

}

void WireClass::writeULongValueToMemory(unsigned long value, byte memaddr) {
  cprValBuffer[memaddr] = (byte)((value & 0xFF000000) >> 24);;
  cprValBuffer[memaddr + 1] = (byte)((value & 0x00FF0000) >> 16 );
  cprValBuffer[memaddr + 2] = (byte)((value & 0x0000FF00) >> 8 );
  cprValBuffer[memaddr + 3] = (byte)((value & 0X000000FF));
}

void WireClass::writeByteValueToMemory(byte value, byte memaddr) {
  cprValBuffer[memaddr] = value;
}

#endif

