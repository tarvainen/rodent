#include "StepperMotor.h"

#ifndef STEPPERMANAGER_H
#define STEPPERMANAGER_H

#define TIMEOUT 5000

class StepperManager {
private:
	StepperMotor *x;
	StepperMotor *y;
	StepperMotor *z;
	int minimumDelayBetweenSteps;
	int delayBetweenSteps;
	long lastStepTimestamp;
	double lastSteppedX;
	double lastSteppedY;
	double lastSteppedZ;
	long timeoutStartTime;
	bool isShuttingDown;
public:
	StepperManager();
	void init();
	StepperMotor *getX() { return this->x; }
	StepperMotor *getY() { return this->y; }
	StepperMotor *getZ() { return this->z; }
	void setX(StepperMotor *motor) { this->x = motor; }
	void setY(StepperMotor *motor) { this->y = motor; }
	void setZ(StepperMotor *motor) { this->z = motor; }
	void setMinimumDelayBetweenSteps(int);
	int getMinimumDelayBetweenSteps();
	void setDelayBetweenSteps(int);
	int getDelayBetweenSteps();
	long getLastStepTime() { return this->lastStepTimestamp; }
	void setLastStepTime(long time) { this->lastStepTimestamp = time; }
	void stepped(long time) { this->lastStepTimestamp = time; }
	bool isTimeToStep(long);
	void disableAll();
	void enableAll();
	double getLastSteppedX() { return this->lastSteppedX; }
	double getLastSteppedY() { return this->lastSteppedY; }
	double getLastSteppedZ() { return this->lastSteppedZ; }
	void setLastSteppedX(double coord) { this->lastSteppedX = coord; }
	void setLastSteppedY(double coord) { this->lastSteppedY = coord; }
	void setLastSteppedZ(double coord) { this->lastSteppedZ = coord; }
	void setDirections(int, int, int);
	int getXStepsPerMM();
	int getYStepsPerMM();
	int getZStepsPerMM();
	void setTimeout(long);
	bool isTimedOut(long);
	bool isOnTimeout();
	void dropTimeout();
};

#endif