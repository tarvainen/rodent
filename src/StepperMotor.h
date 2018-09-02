#include "Arduino.h"
#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H


class StepperMotor {
private:
	int phase;
	long int lastStepTime;
	int delay;
	int stepsPerRevolution;
	int direction;
	int controlPin1;
	int controlPin2;
	int enablePin;
	int phases[4][2] = { { LOW, HIGH }, { HIGH, HIGH }, {HIGH, LOW}, {LOW, LOW} };
	bool isEnabled;
public:
	StepperMotor();
	StepperMotor(int, int, int, int);
	void step();
	void setSpeed(int);
	void setDirection(int);
	int getDirection();
	void enable();
	void disable();
	bool enabled();
	int getStepsPerRevolution() { return this->stepsPerRevolution; }
};

#endif