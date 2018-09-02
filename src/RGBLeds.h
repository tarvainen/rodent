#ifndef RGBLEDS_H
#define RGBLEDS_H

class RGBLeds {
private:
	int _redPin;
	int _greenPin;
	int _bluePin;
	int _redVal;
	int _greenVal;
	int _blueVal;
	bool _on;
public:
	RGBLeds();
	void init();
	void setRedPin(int);
	int getRedPin();
	void setGreenPin(int);
	int getGreenPin();
	void setBluePin(int);
	int getBluePin();
	void setPins(int, int, int);
	void setRedValue(int);
	int getRedValue();
	void setGreenValue(int);
	int getGreenValue();
	void setBlueValue(int);
	int getBlueValue();
	void setValues(int, int, int);
	void on();
	void off();
	bool isOn();
};

#endif