#Robocode
***
*As I remember, this code was written in 2007*
##What is it?
Tank battles game emulator (without GUI)

##Description

* robocode.cpp - Main program file
* robocode.h - Default headers
* types.h - Defines basic data type, classes, varibles and error messages
* types.cpp - Implementation of basic classes

* Makefile - "Make" file for easy "robocode" build
* tests/sample.in - test case input file (used programm by default)
* tests/sample.out - test case output file


##Build
Just type in console:
**make**

Or if you are not using MAKE, type in console follow command:
**g++ -o robocode src/robocode.o**


##Usage
Just type in console:
**robocode** *<:inputfile>*

If **<:inputfile>** not entered, will be used **"sample.in"** by default. File must be placed into the same dir with robocode exec file