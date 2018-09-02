#include "Thread.h"



/*
	Default constructor.
*/
Thread::Thread() {
	this->init();
}

/*
	Takes a pointer of a callback function.
*/
Thread::Thread(void (*function)(void)) {
	this->init();
	this->runnableFunction = function;
}

/*
	Takes a pointer of a callback function, interval and enable value.
*/
Thread::Thread(void (*function)(void), long interval, bool enabled) {
	this->init();
	this->runnableFunction = function;
	this->enabled = enabled;
}

/*
	Initializes thread to it's default values.
*/
void Thread::init() {
	this->runnableFunction = NULL;
	this->enabled = false;
	this->interval = DEFAULT_INTERVAL;
	this->lastRun = -1;
	this->nextRun = -1;
}

/*
	Sets interval for fake thread.
*/
void Thread::setInterval(long interval) {
	interval >= 0 ? this->interval = interval : 0;
}

/*
	Sets the callback function.
*/
void Thread::setFunction(void (*function)(void)) {
	this->runnableFunction = function;
}

/*
	Schedules the next run due to the current time and the interval.
*/
void Thread::scheduleNextRun(long currentTime) {
	if (currentTime < 0) {
		currentTime = millis();
	}
	this->lastRun = currentTime;
	this->nextRun = this->lastRun + this->interval;
}

/*
	Enables the thread execution.
*/
void Thread::enable() {
	this->enabled = true;
}

/*
	Disables the thread execution.
*/
void Thread::disable() {
	this->enabled = false;
}

bool Thread::isTimeToRun(long currentTime) {
	return this->enabled && currentTime >= this->nextRun;
}

/*
	Executes the callback function.
*/
void Thread::run() {
	if (this->runnableFunction != NULL) {
		this->scheduleNextRun(millis());
		this->runnableFunction();
	}
}

long Thread::getInterval() {
	return this->interval;
}

void Thread::setIndex(int index) {
	this->indexInManager = index;
}

int Thread::getIndex() {
	return this->indexInManager;
}