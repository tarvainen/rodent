#include "Arduino.h"
#include "Switch.h"

/*
Default constructor.
*/
Switch::Switch() {
	this->init();
}

/*
Parametric constructor takes a pin number as a parameter.
*/
Switch::Switch(int pin) {
	this->init();
	this->setPin(pin);
}

/*
Initializes all the variables to their default values.
*/
void Switch::init() {
	this->_pin = -1;
}

/*
Sets pin and initializes it as an input and turns pullup resistor high.
*/
void Switch::setPin(int pin) {
	this->_pin = pin;
	pinMode(pin, INPUT); // define to input
	digitalWrite(pin, HIGH); // pullup resistor
}

/*
Returns the pin number (not the digital value of the pin).
*/
int Switch::getPin() {
	return this->_pin;
}

/*
Returns the digital value of the pin.
Returns true if switch is in connected mode and false if it's not.
*/
bool Switch::isOn() {
	if (digitalRead(this->_pin)) {
		return false;
	}
	else {
		return true;
	}
}