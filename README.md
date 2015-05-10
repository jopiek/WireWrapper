## WireWrapper

This example program provides a wrapper around the i2c_t3 library that was created by nox771.
Link to the library:<a href="https://github.com/nox771/i2c_t3">i2c_t3</a>

## Purpose

Purpose of this TestApp is providing a way to manage i2c modules in a scalable way.
The system scans for the available modules.

## Synopsis

myModules is an array of Modules:
* MyModule* myModules = new MyModule[numModules];

The constructor of WireClass uses this WireClass array initializes all modules that we expect to find:

MyModule mainModule  = MyModule(0x40, "Module 1");
myModules[0] = mainModule;

The modules have an address (their i2c address) and a name (to identify them with text).
The modules also have a boolean that indicates if the module isAvailable or not.

* WireClass::initModules() does a scan for all modules that are available and set isAvailable to true if a module was found (default: false).

## Code & Test Example

TestApp.ino itself is a demo of how MyModule and WireClass are being used.

## Motivation

MyModule and WireClass are a testcase for the RobotPatient Patient Simulators.
In future we plan to make the whole project open source, you can already contact us if you are a student of faculty and involved in medical simulation research. 

## Installation

Download the files and open them in Arduino IDE or some other IDE that has support for Teensyduino. Tested on a Teensy 3.1 ARM board.

## API Reference

ToDo: Depending on the size of the project, if it is small and simple enough the reference docs can be added to the README. For medium size to larger projects it is important to at least provide a link to where the API reference docs live.

## Contributors

- Library from nox771 (<a href="https://github.com/nox771/i2c_t3">link</a>)
- kpc, defragster (see: <a href="https://forum.pjrc.com/threads/28560-Teensy-3-x-and-Array-of-Objects?p=72549#post72549">pjrc.com</a>)

## License

These files are released under GNU General Public License v3.0. 
The GPLv3 allows others to copy, distribute and modify the software as long as they state what has been changed when, 
and ensure that any modifications are also licensed under the GPL. 

Software incorporating (via compiler) GPL-licensed code must also be made also available under the GPLv3 along with build & install instructions.
