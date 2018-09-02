#ifndef EEPROM_WRITER_H
#define EEPROM_WRITER_H
#include "RodentStatus.h"
#include "EEPROM.h"

class EEPROMWriter {

public:
	static void write(StatusObject);
	static StatusObject read();
	static void dumpMemory();
};

#endif