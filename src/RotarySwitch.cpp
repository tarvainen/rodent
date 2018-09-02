#include "RotarySwitch.h"

/*
Default constructor.
*/
RotarySwitch::RotarySwitch() {
	this->init();
}

/*
Parametric constructor takes the analog input pin as a parameter.
*/
RotarySwitch::RotarySwitch(int pin) {
	this->init();
	this->setPin(pin);
}

/*
Initializes all the variables to their default values.
*/
void RotarySwitch::init() {
	this->_pin = -1;
	this->_currentPosition = -1;
}

/*
Sets analog input pin.
*/
void RotarySwitch::setPin(int pin) {
	this->_pin = pin;
	pinMode(pin, INPUT);
}

/*
Returns analog input pin number.
*/
int RotarySwitch::getPin() {
	return this->_pin;
}

/*
Returns current switch position. If position is not detected, returns -1.
*/
int RotarySwitch::getCurrentPosition() {
	return this->_currentPosition;
}

/*
Checks analog pin value and changes _currentPosition variable to determine the position of the switch.
Analog value is changed by the rotary switch connected to the resistor ladder (voltage division).
*/
void RotarySwitch::checkInputValue() {
	if (this->_pin >= 0) {
		int value = analogRead(this->_pin);
		//Serial.println(value);

		if (value >= 900) {
			this->_currentPosition = 2;
		}
		else if (value < 50) {
			this->_currentPosition = 0;
		}
		else if (value >= 400 && value <= 600) {
			this->_currentPosition = 1;
		}
	}
}
