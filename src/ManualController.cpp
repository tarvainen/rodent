#include "ManualController.h"

/*
Default constructor.
*/
ManualController::ManualController() {
	this->init();
}

/*
Initializes all the variables to their default values.
*/
void ManualController::init() {
	this->encoder = new RotaryEncoder();
	this->leftSwitch = new RotarySwitch();
	this->rightSwitch = new RotarySwitch();
}

/*
Sets rotary encoder object.
*/
void ManualController::setRotaryEncoder(RotaryEncoder *encoder) {
	this->encoder = encoder;
}

/*
Returns the rotary encoder object.
*/
RotaryEncoder* ManualController::getRotaryEncoder() {
	return this->encoder;
}

/*
Sets the left rotary switch object.
*/
void ManualController::setLeftSwitch(RotarySwitch* _switch) {
	this->leftSwitch = _switch;
}

/*
Returns the left rotary switch object.
*/
RotarySwitch* ManualController::getLeftSwitch() {
	return this->leftSwitch;
}

/*
Sets the right rotary switch object.
*/
void ManualController::setRightSwitch(RotarySwitch* _switch) {
	this->rightSwitch = _switch;
}

/*
Returns the right rotary switch object.
*/
RotarySwitch* ManualController::getRightSwitch() {
	return this->rightSwitch;
}