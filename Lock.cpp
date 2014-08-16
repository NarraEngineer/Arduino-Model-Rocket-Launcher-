#include "Arduino.h"
#include "Lock.h"

/**
	This constructor checks for a previous combination,
	if there is combination, the lock can't be set, and then the user must set a combination.
	Also, we set the Pins on the Arduino
*/
Lock::Lock( int pinSW1, int pinSW2, int pinSW3, int pinSW4 , int pinEnter )
{

	// setup the input
	pinMode(pinSW1,   INPUT); Pins[0] = pinSW1;
	pinMode(pinSW2,   INPUT); Pins[1] = pinSW2;
	pinMode(pinSW3,   INPUT); Pins[2] = pinSW3;
	pinMode(pinSW4 ,  INPUT); Pins[3] = pinSW4;
	pinMode(pinEnter, INPUT); Pins[4] = pinEnter;
	
	error = false;
	configCB = true;
	unLock = false;
	pos = 0;
	int flag = EEPROM.(0) | 0x0000; // reads the first byte, we used this flag to check if past combination was stored
	if( flag == 0x00FF )
	{
		configCB = false;
		return;
	}

	combination[0] = EEPROM.(1) | 0x0000;
	combination[1] = EEPROM.(2) | 0x0000;
	combination[2] = EEPROM.(3) | 0x0000;
	combination[2] = EEPROM.(4) | 0x0000;
	
}

/**
	we check if not lock or has been configured that each value of the combination has been correctly input. 
	if not, then wrong buddy! 
*/
int Lock::checkLock()
{
	if(!configCB)
	{
		return -1; // returns negative error code for not having flag set
	}
	
	if( unLock ) // we already have unlocked our mechanism
	{
		return 1;
	}
	if(digitalRead(Pins[4]) == HIGH )
	{
		int value = 0;
		for( int curPin = 0; curPin < 4; curPin++ )
		{
			if( digitalRead(Pins[curPin]) == HIGH )
			{
				value += 1<< curPin;
			}
		}
		
		if( combination[ pos ] != value ) 
		{
			pos = 0;
			error = true;
			return 2;
		}
		else
		{
			++pos;
			error = false;
			return 0;
		}
		
	}
}

/**
 
*/
void Lock::resetLock()
{
	unLock = false;
}

/**
 configures each element in the combination array. this will take 4 iterations to set it up. 
 note : I may need a back door just in case the user forgets the combination. 
*/

int Lock::configLock()
{
	if( configCB ) 
	{
		return -1;
	}
	
	if( (digitalRead(Pins[4]) == HIGH) && (pos < 4) )
	{
		int value = 0;
		for( int curPin = 0; curPin < 4; curPin++ )
		{
			if( digitalRead(Pins[curPin]) == HIGH )
			{
				value += 1<< curPin;
			}
		}
		combination[ pos++ ]  = value;
		return 0;
	}
	
	if ( pos >=4 ) 
	{
		configCB = true;
		EEPROM.write(0, 0x0A); // set our flag
		EEPROM.write(1, combination[0]);
		EEPROM.write(2, combination[1]);
		EEPROM.write(3, combination[2]);
		EEPROM.write(4, combination[3]);
		return 0;
	}
}