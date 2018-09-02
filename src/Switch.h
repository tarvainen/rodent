#ifndef SWITCH_H
#define SWITCH_H

class Switch {
private:
	int _pin;
	void init();
public:
	Switch();
	Switch(int);
	void setPin(int);
	int getPin();
	bool isOn();
};

#endif