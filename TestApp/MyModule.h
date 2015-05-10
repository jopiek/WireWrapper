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

#include "Arduino.h"

#ifndef MyModule_h
#define MyModule_h

class MyModule {

  private:
    boolean _isAvailable;
  public:
    void print();
    void printName();
    void setAvailability(boolean isAvailable);
    boolean isAvailable();
    
    MyModule();
    MyModule(byte _id, char* _name);

    /*
       Public:
       niet heel erg mooi, maar in dit geval
       zijn getters / setters wat overdreven
    */
    byte id;
    char *name;

};

#endif
