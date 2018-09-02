#ifndef MANUALCONTROLLER_H
#define MANUALCONTROLLER_H

#include "RotaryEncoder.h"
#include "RotarySwitch.h"

class ManualController {
private:
	void init();

	RotaryEncoder *encoder;
	RotarySwitch *leftSwitch;
	RotarySwitch *rightSwitch;

public:
	ManualController();
	void setRotaryEncoder(RotaryEncoder*);
	RotaryEncoder* getRotaryEncoder();
	void setLeftSwitch(RotarySwitch*);
	RotarySwitch* getLeftSwitch();
	void setRightSwitch(RotarySwitch*);
	RotarySwitch* getRightSwitch();
};

#endif