#include "EEPROMWriter.h"

/*
Reads an Rodent status object from EEPROM memory.
*/
StatusObject EEPROMWriter::read() {
	StatusObject status;
	EEPROM.get(0, status);
	return status;
}

/*
Writes the given Rodent status object to the EEPROM memory.
*/
void EEPROMWriter::write(StatusObject statusObject) {
	EEPROM.put(0, statusObject);
}

/*
Dumps the EEPROM memory.
*/
void EEPROMWriter::dumpMemory() {

}