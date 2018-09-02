#include "Spindle.h"
#include "Arduino.h"

/*
Default constructor.
*/
Spindle::Spindle() {
	this->init();
}

/*
Initializes all the variables to their default values.
*/
void Spindle::init() {
	this->pwmPin = -1;
	this->dirPin = -1;
	this->dir = 1;
	this->currentSpeed = 0.0;
	this->targetSpeed = 0.0;
	this->pulsesPerRevolution = 1;
	this->sensorPin = -1;
	this->spinning = false;
	this->pwmValue = 0;
}

/*
Sets spindle's pwm pin and initializes it as an output.
*/
void Spindle::setPWMPin(int pin) {
	this->pwmPin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	analogWrite(pin, 0);
}

/*
Returns spindle's pwm pin.
*/
int Spindle::getPWMPin() {
	return this->pwmPin;
}

/*
Sets spindle's direction pin and initializes it as an output.
*/
void Spindle::setDirectionPin(int pin) {
	this->dirPin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
}

/*
Returns spindle's direction pin.
*/
int Spindle::getDirectionPin() {
	return this->dirPin;
}

/*
Sets spindle's sensor pin and initializes it as an input.
Pullup resistor is up by default.
*/
void Spindle::setSensorPin(int pin) {
	this->sensorPin = pin;
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH); // pullup resistor up
}

/*
Returns spindle's sensor pin.
*/
int Spindle::getSensorPin() {
	return this->sensorPin;
}

/*
Sets value indicating how many pulses the spindle will generate in one revolution.
*/
void Spindle::setPulsesPerRevolution(int pulses) {
	this->pulsesPerRevolution = pulses;
}

/*
Returns spindle's generated pulses per revolution.
*/
int Spindle::getPulsesPerRevolution() {
	return this->pulsesPerRevolution;
}

/*
Returns true if the spindle is spinning.
*/
bool Spindle::isSpinning() {
	return this->spinning;
}

/*
Stops the spindle.
*/
void Spindle::stop() {
	this->spinning = false;
	this->setPWM(0);
}

/*
Puts the spindle spinning. The speed will be either full speed
or the speed defined by another function.
*/
void Spindle::spin() {
	this->spinning = true;
	this->setPWM(255);
}

/*
Sets spindle's direction to clockwise direction.
*/
void Spindle::goClockwise() {
	this->dir = 1;
}

/*
Sets spindle's direction to counter-clockwise direction.
*/
void Spindle::goCounterClockwise() {
	this->dir = -1;
}

/*
Sets the target spinning speed of the spindle.
Unit is RPM.
*/
void Spindle::setTargetSpeed(double speed) {
	if (speed >= 0) {
		this->targetSpeed = speed;
	}
	else {
		this->targetSpeed = 0;
	}
}

/*
Returns the spindle's target speed (RPM).
*/
double Spindle::getTargetSpeed() {
	return this->targetSpeed;
}

/*
Sets the current speed of the spindle (RPM).
*/
void Spindle::setCurrentSpeed(double speed) {
	if (speed >= 0) {
		this->currentSpeed = speed;
	}
	else {
		this->currentSpeed = 0;
	}
}

/*
Returns the current speed of the spindle.
*/
double Spindle::getCurrentSpeed() {
	return this->currentSpeed;
}

/*
Sets the pwm value of the motor enable pin.
*/
void Spindle::setPWM(int value) {
	if (value < 0) {
		value = 0;
	}
	else if (value > 255) {
		value = 255;
	}
	this->pwmValue = value;
	analogWrite(this->pwmPin, value);
}

/*
Returns the pwm value of the motor enable pin.
*/
int Spindle::getPWM() {
	return this->pwmValue;
}