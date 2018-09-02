#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#define THREAD_MAX_AMOUNT 20

#include "Thread.h"
class ThreadManager {
private:

	void formatThreads();
	int indexOfLast();
public:
	Thread *threads[THREAD_MAX_AMOUNT];
	ThreadManager();
	int add(Thread*);
	void remove(Thread);
	void runThreads();
	Thread* getThreads();

};

#endif