#include "StepperMotor.h"
#include "Arduino.h"

/*
	Default constructor.
*/
StepperMotor::StepperMotor() {
	this->controlPin1 = 0;
	this->controlPin2 = 0;
	this->enablePin = 0;
	this->delay = 0;
	this->phase = 0;
	this->stepsPerRevolution = 0;
	this->direction = 0;
	this->lastStepTime = 0;
}


StepperMotor::StepperMotor(int controlPin1, int controlPin2, int enablePin, int stepsPerRevolution) {
	// init pins
	this->controlPin1 = controlPin1;
	this->controlPin2 = controlPin2;
	this->enablePin = enablePin;
	pinMode(this->controlPin1, OUTPUT);
	pinMode(this->controlPin2, OUTPUT);
	pinMode(this->enablePin, OUTPUT);
	digitalWrite(this->controlPin1, LOW);
	digitalWrite(this->controlPin2, LOW);
	digitalWrite(this->enablePin, LOW);
	
	// init other vars
	this->delay = 100;
	this->lastStepTime = millis();
	this->phase = 0;
	this->stepsPerRevolution = stepsPerRevolution;
	this->direction = -1;
}

/*
	Sets the right delay for the stepper's stepping.
*/
void StepperMotor::setSpeed(int speed) {
	this->delay = 60 / 1000 / this->stepsPerRevolution / speed;
}

void StepperMotor::step() {
	this->phase += this->direction;

	if (this->phase < 0) {
		this->phase = 3;
	}
	else if (this->phase > 3) {
		this->phase = 0;
	}
	digitalWrite(this->controlPin1, this->phases[this->phase][0]);
	digitalWrite(this->controlPin2, this->phases[this->phase][1]);
}

/*
	Sets the direction of the stepper motor.
*/
void StepperMotor::setDirection(int direction) {
	if (direction > 0) {
		this->direction = 1;
	}
	else if (direction < 0) {
		this->direction = -1;
	}
}

int StepperMotor::getDirection() {
	return this->direction;
}
void StepperMotor::enable() {
	digitalWrite(this->enablePin, HIGH);
	this->isEnabled = true;
}

void StepperMotor::disable() {
	digitalWrite(this->enablePin, LOW);
	this->isEnabled = false;
}

bool StepperMotor::enabled() {
	return this->isEnabled;
}