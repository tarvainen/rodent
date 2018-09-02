#include "RotaryEncoder.h"
#include "Arduino.h"
/*
	Default constructor.
*/
RotaryEncoder::RotaryEncoder() {
	this->init();
}

/*
	Constructor for encoder without push button.
*/
RotaryEncoder::RotaryEncoder(int signalPin1, int signalPin2) {
	this->init();
	this->setSignalPin1(signalPin1);
	this->setSignalPin2(signalPin2);
}

/*
	Constructor for encoder with push button.
*/
RotaryEncoder::RotaryEncoder(int signalPin1, int signalPin2, int pushButtonPin) {
	this->init();
	this->setSignalPin1(signalPin1);
	this->setSignalPin2(signalPin2);
	this->setPushButtonPin(pushButtonPin);
	this->singlePhaseInterrupt = false;
}

/*
	Initializes all the components.
*/
void RotaryEncoder::init() {
	this->currentValue = 0;
	this->lastValue = 0;
	this->lastReadValue = 0;
	this->isEnabled = true;
	this->onPositiveDetect = NULL;
	this->onNegativeDetect = NULL;
	this->signalPin1 = 0;
	this->signalPin2 = 0;
	this->pushButtonPin = 0;
}

/*
	Initializes the input signal pin 1.
*/
void RotaryEncoder::setSignalPin1(int signalPin1) {
	this->signalPin1 = signalPin1;
	pinMode(this->signalPin1, INPUT);
	digitalWrite(this->signalPin1, HIGH); // turn pullup resistor on
}

/*
	Initializes the input signal pin 2.	
*/
void RotaryEncoder::setSignalPin2(int signalPin2) {
	this->signalPin2 = signalPin2;
	pinMode(this->signalPin2, INPUT);
	digitalWrite(this->signalPin2, HIGH); // turn pullup resistor on
}

/*
	Initiali<es the push button pin.
*/
void RotaryEncoder::setPushButtonPin(int pushButtonPin) {
	this->pushButtonPin = pushButtonPin;
	pinMode(this->pushButtonPin, INPUT);
	digitalWrite(this->pushButtonPin, HIGH); // turn pullup resistor on
}

int RotaryEncoder::getSignalPin1() {
	return this->signalPin1;
}

int RotaryEncoder::getSignalPin2() {
	return this->signalPin2;
}

int RotaryEncoder::getPushButtonPin() {
	return this->pushButtonPin;
}

/*
	Sets the function's, which is run when rotation is detected, pointer.
*/
void RotaryEncoder::onPositiveRotation(void(*function)(void)) {
	this->onPositiveDetect = function;
}

/*
Sets the function's, which is run when rotation is detected, pointer.
*/
void RotaryEncoder::onNegativeRotation(void(*function)(void)) {
	this->onNegativeDetect = function;
}
/*
	Function, which is run when rotation is detected.
*/
void RotaryEncoder::registerRotation() {

	int msb = digitalRead(this->signalPin1);
	int lsb = digitalRead(this->signalPin2);

	int encoded = (msb << 1) | lsb;
	int sum = (this->lastValue << 2) | encoded;
	if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
		if (this->onPositiveDetect != NULL) {
			this->onPositiveDetect();
		}
		this->currentValue--;
	}

	else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
		if (this->onNegativeDetect != NULL) {
			this->onNegativeDetect();
		}
		this->currentValue++;
	}
	this->lastValue = encoded;


}

bool RotaryEncoder::isPushButtonPressed() {
	return this->pushButtonPressed;
}

void RotaryEncoder::setPushButtonStatus(bool pressed) {
	this->pushButtonPressed = pressed;
}

/*
	Returns the current value of the encoder.
*/
long RotaryEncoder::getCurrentValue() {
	return this->currentValue;
}

/*
	Set's current value of the encoder.
*/
void RotaryEncoder::setCurrentValue(long currentValue) {
	this->currentValue = currentValue;
}

/*
	Returns last value of the encoder.
*/
long RotaryEncoder::getLastValue() {
	return this->lastValue;
}

/*
	Returns true if the encoder is enabled, else returns false.
*/
bool RotaryEncoder::enabled() {
	return this->isEnabled;
}

/*
	Enables the encoder.
*/
void RotaryEncoder::enable() {
	this->isEnabled = true;
}

/*
	Disables the encoder.
*/
void RotaryEncoder::disable() {
	this->isEnabled = false;
}

void RotaryEncoder::flip() {
	int temp = this->signalPin2;
	this->setSignalPin2(this->signalPin1);
	this->setSignalPin1(temp);
}

long RotaryEncoder::read() {
	long val = this->currentValue;
	long lVal = this->lastReadValue;
	this->lastReadValue = val;
	return val - lVal;
}