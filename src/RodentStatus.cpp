#include "RodentStatus.h"

/*
Default constructor.
*/
RodentStatus::RodentStatus() {
	this->init();
}

/*
Initializes all the variables to their default values.
*/
void RodentStatus::init() {
	this->machineCoordinateSystem = new CoordinateSystem();
	this->_absoluteProgramming = false;
	this->_lightsOn = false;
	this->_red = 0;
	this->_green = 0;
	this->_blue = 0;
	this->_soundsOn = false;
}

/*
Sets coordinate system to the status object.
*/
void RodentStatus::setCoordinateSystem(CoordinateSystem *system) {
	this->machineCoordinateSystem = system;
}

/*
Returns coordinate system.
*/
CoordinateSystem* RodentStatus::getCoordinateSystem() {
	return this->machineCoordinateSystem;
}

/*
Sets absolute programming state.
*/
void RodentStatus::setAbsoluteProgrammingState(bool absolute) {
	this->_absoluteProgramming = absolute;
}

/*
Returns absolute programming state.
*/
bool RodentStatus::getAbsoluteProgrammingState() {
	return this->_absoluteProgramming;
}

/*
Sets light state.
*/
void RodentStatus::setLightsState(bool lightsOn) {
	this->_lightsOn = lightsOn;
}

/*
Returns light state.
*/
bool RodentStatus::getLightState() {
	return this->_lightsOn;
}

/*
Sets red led pwm value.
*/
void RodentStatus::setRedValue(int value) {
	this->_red = value;
}

/*
Returns red led pwm value.
*/
int RodentStatus::getRedValue() {
	return this->_red;
}

/*
Sets green led pwm value.
*/
void RodentStatus::setGreenValue(int value) {
	this->_green = value;
}

/*
Returns green led pwm value.
*/
int RodentStatus::getGreenValue() {
	return this->_green;
}

/*
Sets blue led pwm value.
*/
void RodentStatus::setBlueValue(int value) {
	this->_blue = value;
}

/*
Returns blue led pwm value.
*/
int RodentStatus::getBlueValue() {
	return this->_blue;
}

/*
Sets sound state.
*/
void RodentStatus::setSoundState(bool soundOn) {
	this->_soundsOn = soundOn;
}

/*
Returns saved sound state.
*/
bool RodentStatus::getSoundState() {
	return this->_soundsOn;
}