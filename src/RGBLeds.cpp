#include "RGBLeds.h"
#include "Arduino.h"

/*
Default constructor.
*/
RGBLeds::RGBLeds() {
	this->init();
}

/*
Initializes all the variables to their default values.
*/
void RGBLeds::init() {
	this->_redPin = -1;
	this->_greenPin = -1;
	this->_bluePin = -1;
	this->_redVal = 0;
	this->_greenVal = 0;
	this->_blueVal = 0;
	this->_on = false;
}

/*
Sets red channel's pin.
*/
void RGBLeds::setRedPin(int pin) {
	if (pin > 0) {
		this->_redPin = pin;
		pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH);
		analogWrite(this->_redPin, 0);
	}
}

/*
Returns red channel's pin.
*/
int RGBLeds::getRedPin() {
	return this->_redPin;
}

/*
Sets green channel's pin.
*/
void RGBLeds::setGreenPin(int pin) {
	if (pin > 0) {
		this->_greenPin = pin;
		pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH);
		analogWrite(this->_greenPin, 0);
	}
}

/*
Returns green channel's pin.
*/
int RGBLeds::getGreenPin() {
	return this->_greenPin;
}

/*
Sets blue channel's pin.
*/
void RGBLeds::setBluePin(int pin) {
	if (pin > 0) {
		this->_bluePin = pin;
		pinMode(pin, OUTPUT);
		digitalWrite(this->_bluePin, HIGH);
		analogWrite(this->_bluePin, 0);
	}
}

/*
Returns green channel's pin.
*/
int RGBLeds::getBluePin() {
	return this->_bluePin;
}

/*
Sets all the pins.
*/
void RGBLeds::setPins(int r, int g, int b) {
	this->setRedPin(r);
	this->setGreenPin(g);
	this->setBluePin(b);
}

/*
Sets red channel's pwm.
*/
void RGBLeds::setRedValue(int val) {
	if (this->_redPin != -1 && val >= 0 && val <= 255) {
		int write = this->_on ? val : 0;
		analogWrite(this->_redPin, write);
		this->_redVal = val;
	}
}

/*
Returns red channel's pwm value.
*/
int RGBLeds::getRedValue() {
	return this->_redVal;
}

/*
Sets green channel's pwm.
*/
void RGBLeds::setGreenValue(int val) {
	if (this->_greenPin != -1 && val >= 0 && val <= 255) {
		int write = this->_on ? val : 0;
		analogWrite(this->_greenPin, write);
		this->_greenVal = val;
	}
}

/*
Returns green channel's pwm value.
*/
int RGBLeds::getGreenValue() {
	return this->_greenVal;
}

/*
Sets blue channel's pwm.
*/
void RGBLeds::setBlueValue(int val) {
	if (this->_bluePin != -1 && val >= 0 && val <= 255) {
		int write = this->_on ? val : 0;
		analogWrite(this->_bluePin, write);
		this->_blueVal = val;
	}
}

/*
Returns blue channel's pwm value.
*/
int RGBLeds::getBlueValue() {
	return this->_blueVal;
}

/*
Writes last values to every channel.
*/
void RGBLeds::on() {
	this->_on = true;
	if (this->_redPin != -1) {
		analogWrite(this->_redPin, this->_redVal);
	}
	if (this->_greenPin != -1) {
		analogWrite(this->_greenPin, this->_greenVal);
	}
	if (this->_bluePin != -1) {
		analogWrite(this->_bluePin, this->_blueVal);
	}
}

/*
Writes 0 to every channel.
*/
void RGBLeds::off() {
	this->_on = false;
	if (this->_redPin != -1) {
		analogWrite(this->_redPin, 0);
	}
	if (this->_greenPin != -1) {
		analogWrite(this->_greenPin,0);
	}
	if (this->_bluePin != -1) {
		analogWrite(this->_bluePin, 0);
	}
}

/*
Returns true if light's are on and false if they are not.
*/
bool RGBLeds::isOn() {
	return this->_on;
}