#include "ThreadManager.h"
#include "Arduino.h"

/*
	Default constructor.
*/
ThreadManager::ThreadManager() {
	this->formatThreads();
}

/*
	Sets all threads to null pointer.
*/
void ThreadManager::formatThreads() {
	for (int i = 0; i < THREAD_MAX_AMOUNT; i++) {
		this->threads[i] = NULL;
	}
}

int ThreadManager::add(Thread *thread) {
	int position = this->indexOfLast();
	if (position != -1) {
		this->threads[position] = thread;
		thread->setIndex(position);
		return position;
	}
	return -1;
}

int ThreadManager::indexOfLast() {
	for (int i = 0; i < THREAD_MAX_AMOUNT; i++) {
		if (this->threads[i] == NULL) {
			return i;
		}
	}
	return -1;
}

void ThreadManager::remove(Thread thread) {

}

void ThreadManager::runThreads() {
	char last = this->indexOfLast();
	for (int i = 0; i < last; i++) {
		if (this->threads[i]->isTimeToRun(millis())) {
			this->threads[i]->run();
		}
	}
}

Thread* ThreadManager::getThreads() {
	return *(this->threads);
}