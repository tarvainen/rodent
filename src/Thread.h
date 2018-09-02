#ifndef THREAD_H
#define THREAD_H


#include "Arduino.h"

#define DEFAULT_INTERVAL 100 // default interval in milliseconds

class Thread {
private:
	long interval;
	long lastRun;
	long nextRun;
	bool enabled;
	void scheduleNextRun(long);
	void(*runnableFunction)(void);
	int indexInManager;

public:
	Thread();
	Thread(void (*function)(void));
	Thread(void (*function)(void), long, bool);
	void setInterval(long);
	void enable();
	void disable();
	void setFunction(void (*function)(void));
	void run();
	void init();
	bool isTimeToRun(long);
	long getInterval();
	void setIndex(int);
	int getIndex();
};

#endif