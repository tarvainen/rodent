#ifndef GCODEPARSER_H
#define GCODEPARSER_H
#include "GCode.h"
class GCodeParser {
private:
	int maxInputLength;

public:
	GCodeParser();
	void setMaximumInputLength(int len) { this->maxInputLength = len; }
	GCode* parse(char[]);
};


#endif