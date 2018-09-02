#include "Arduino.h"
#include "Speaker.h"

/*
Default constructor.
*/
Speaker::Speaker() {
	this->init();
}

/*
Parametric constructor takes the tone pin as a parameter.
*/
Speaker::Speaker(int pin) {
	this->init();
	this->setPin(pin);
}

/*
Initializes all the variables to their default values.
*/
void Speaker::init() {
	this->_pin = -1;
	this->_currentSound = NULL;
	this->_lastChange = 0;
	this->_nextChange = 0;
	this->_muted = false;
}

/*
Sets tone pin and initializes it.
*/
void Speaker::setPin(int pin) {
	this->_pin = pin;
	pinMode(pin, OUTPUT);
}

/*
Returns tone pin.
*/
int Speaker::getPin() {
	return this->_pin;
}

/*
Sets current sound to given Sound object.
*/
void Speaker::play(Sound *sound) {
	this->_currentSound = sound;
}

/*
Returns currently played sound object.
*/
Sound* Speaker::getSound() {
	return this->_currentSound;
}

/*
Unmutes the speaker.
*/
void Speaker::unmute() {
	this->_muted = false;
}

/*
Mutes the speaker.
*/
void Speaker::mute() {
	this->_muted = true;
}

/*
Returns true if the speaker is muted and false if it's not.
*/
bool Speaker::muted() {
	return this->_muted;
}

/*
Returns true if it's time to make some change in speaker's tone.
*/
bool Speaker::isTimeToChange() {
	return millis() >= this->_nextChange && this->_currentSound != NULL;
}

/*
Makes a change in the speaker's tone.
*/
void Speaker::goOn() {
	int note = this->_currentSound->getCurrentNote();
	int duration = this->_currentSound->getCurrentDuration();
	if (note == -1 || duration == -1) {
		this->_currentSound = NULL;
		return;
	}
	if (!this->_muted) {
		if (note == -2) {
			noTone(this->_pin);
		}
		else {
			tone(this->_pin, note, duration);
		}
	}
	this->_nextChange = millis() + this->_currentSound->getCurrentDuration();
	this->_currentSound->next();
}