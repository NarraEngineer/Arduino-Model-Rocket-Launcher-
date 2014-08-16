/**
	Lock.h
	@author  - Justin Barnes
	@version - 1.0 8/1/2014
*/

#ifndef LOCK_H
#define LOCK_H
#include "Arduino.h"
#include "EEPROM.h"
class Lock
{
public:
	Lock( int pinSW1, int pinSW2, int pinSW3, int pinSW4 , int pinEnter );
	int checkLock();
	void resetLock();
	int configLock();
	boolean isCorrect(); // for checking each value of the combination
	boolean isUnLocked();
private:
	boolean unLock;
	boolean configCB; 
	boolean error;
	int combination [4];
	int Pins[5];
	int pos;
};
#endif