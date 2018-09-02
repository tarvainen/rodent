#ifndef SPEAKER_H
#define SPEAKER_H

#include "Sound.h"

class Speaker {
private:
	int _pin;
	bool _muted;
	Sound *_currentSound;
	long _lastChange; // timestamp for last change
	long _nextChange;
public:
	Speaker();
	Speaker(int);
	void init();
	void setPin(int);
	int getPin();
	void setSound(Sound*);
	Sound *getSound();
	void unmute();
	void mute();
	bool muted();
	bool isTimeToChange();
	void goOn();
	void play(Sound*);
};

#endif