#ifndef ROTARY_SWITCH_H
#define ROTARY_SWITCH_H

#include "Arduino.h"

class RotarySwitch {
private:
	int _pin;
	int _currentPosition;

	void init();

public:
	RotarySwitch();
	RotarySwitch(int);
	void setPin(int);
	int getPin();
	int getCurrentPosition();
	void checkInputValue();
};

#endif