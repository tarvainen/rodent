#include "Arduino.h"
#include "Sound.h"

/*
Default constructor.
*/
Sound::Sound() {
	this->init();
}

/*
Parametric constructor takes a integer array pointer as a parameter.
Array must have the frequencies row and the duration row for each sound.
*/
Sound::Sound(int *opts) {
	this->init();
	this->setOptions(opts);
}

/*
Sets the options values.
*/
void Sound::setOptions(int *opts) {
	if (opts != NULL) {
		this->_opts = opts;
	}
}

/*
Initializes all the variables to their default values.
*/
void Sound::init() {
	this->_opts = NULL;
	this->_currentNote = 0;
}

/*
Returns the frequency value of the current sound.
*/
int Sound::getCurrentNote() {
	if (this->_opts[this->_currentNote] != NULL) {
		return this->_opts[this->_currentNote];
	}
	
	return -1;
}

/*
Returns the duration of the current note.
*/
int Sound::getCurrentDuration() {
	if (this->_opts[this->_currentNote + 1] != NULL) {
		return this->_opts[this->_currentNote + 1];
	}

	return -1;
}

/*
Goes to the next element of the sound (increments the position value).
*/
void Sound::next() {
	this->_currentNote += 2;
}