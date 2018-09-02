#include "StepperManager.h"
#include "StepperMotor.h"

/*
Default constructor.
*/
StepperManager::StepperManager() {
	this->init();
}

/*
Initializes all the values to their defaults.
*/
void StepperManager::init() {
	&x != NULL ? this->x = x : this->x = new StepperMotor();
	&y != NULL ? this->y = y : this->y = new StepperMotor();
	&z != NULL ? this->z = z : this->z = new StepperMotor();
	this->lastSteppedX = 0;
	this->lastSteppedY = 0;
	this->lastSteppedZ = 0;
	this->lastStepTimestamp = 0.0;
	this->isShuttingDown = false;
	this->delayBetweenSteps = 0;
	this->timeoutStartTime = 0;
}

/*
Sets the minimum delay between stepper motor steps assuming all the motor are the same kind.
*/
void StepperManager::setMinimumDelayBetweenSteps(int microseconds) {
	this->minimumDelayBetweenSteps = microseconds;
}

/*
Returns the minimum delay between steps.
*/
int StepperManager::getMinimumDelayBetweenSteps() {
	return this->minimumDelayBetweenSteps;
}

/*
Sets delay between stepper motor steps assuming all the motors are the same kind.
*/
void StepperManager::setDelayBetweenSteps(int microseconds) {
	this->delayBetweenSteps = microseconds;
}

/*
Returns the delay between steps.
*/
int StepperManager::getDelayBetweenSteps() {
	return this->delayBetweenSteps;
}

/*
Disables all the motors.
*/
void StepperManager::disableAll() {
	this->x->disable();
	this->y->disable();
	this->z->disable();
}

/*
Enables all the motors.
*/
void StepperManager::enableAll() {
	this->x->enable();
	this->y->enable();
	this->z->enable();
	this->isShuttingDown = false;
}

/*
Sets motor directions for all the motors got.
*/
void StepperManager::setDirections(int x, int y, int z) {
	this->x->setDirection(x);
	this->y->setDirection(y);
	this->z->setDirection(z);
}

/*
Sets timeout for shutting down the power from motors. "Motor hibernate".
*/
void StepperManager::setTimeout(long time) {
	this->timeoutStartTime = time;
	this->isShuttingDown = true;
}

/*
Returns true if its time to go sleep with the motors.
*/
bool StepperManager::isTimedOut(long time) {
	if (time - this->timeoutStartTime >= TIMEOUT) {
		this->isShuttingDown = false;
		return true;
	}
	else {
		return false;
	}
}

/*
Returns true if the motors are going to hibernate.
*/
bool StepperManager::isOnTimeout() {
	return this->isShuttingDown;
}

/*
Deletes the timeout so the motors wont stop.
*/
void StepperManager::dropTimeout() {
	this->isShuttingDown = false;
}