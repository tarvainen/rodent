#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

class RotaryEncoder {
private:
	volatile long currentValue; // contains current position of the encoder
	volatile long lastReadValue; // contains the last value readed
	volatile long lastValue; // contains last position of the encoder
	bool pushButtonPressed;
	int signalPin1;
	int signalPin2;
	int pushButtonPin;
	bool isEnabled;
	void(*onPositiveDetect)(void); // function to run when rotation detected
	void(*onNegativeDetect)(void);
	void init();
	bool singlePhaseInterrupt; // if uses only single interrupt pin

public:
	RotaryEncoder(); // default constructor
	RotaryEncoder(int, int); // constructor for encoder without push button
	RotaryEncoder(int, int, int); // constructor for encoder with push button
	void setSignalPin1(int);
	void setSignalPin2(int);
	void setPushButtonPin(int);
	int getSignalPin1();
	int getSignalPin2();
	int getPushButtonPin();
	bool isPushButtonPressed();
	void setPushButtonStatus(bool);
	void onPositiveRotation(void(*function)(void));
	void onNegativeRotation(void(*function)(void));
	void registerRotation();
	long getCurrentValue();
	long getLastValue();
	void reset();
	void setCurrentValue(long);
	bool enabled();
	void enable();
	void disable();
	void flip();
	long read();
	long getLastReadValue() { return this->lastReadValue; }
	void setToSinglePhase() { this->singlePhaseInterrupt = true; }
};

#endif