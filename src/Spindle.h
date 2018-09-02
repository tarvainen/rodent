#ifndef SPINDLE_H
#define SPINDLE_H

class Spindle {
private:
	int pwmPin;
	int dirPin;
	int sensorPin;
	int pulsesPerRevolution;
	int dir;
	bool spinning;
	double targetSpeed;
	double currentSpeed;
	int pwmValue;
public:
	Spindle();
	void init();
	void setPWMPin(int);
	int getPWMPin();
	void setDirectionPin(int);
	int getDirectionPin();
	void setSensorPin(int);
	int getSensorPin();
	void setPulsesPerRevolution(int);
	int getPulsesPerRevolution();
	bool isSpinning();
	void stop();
	void spin();
	void goClockwise();
	void goCounterClockwise();
	void setTargetSpeed(double);
	double getTargetSpeed();
	void setCurrentSpeed(double);
	double getCurrentSpeed();
	void setPWM(int);
	int getPWM();
};

#endif