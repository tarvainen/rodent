#ifndef SOUND_H
#define SOUND_H

class Sound {
private:
	int *_opts; // options array pointer
	int _currentNote;

public:
	Sound();
	Sound(int*);
	void init();
	int getCurrentNote();
	int getCurrentDuration();
	void next();
	void setOptions(int*);
};

#endif