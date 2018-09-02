#include "GCodeParser.h"


char *flip(char *str, int len) {
	char *result = new char[len + 1];
	for (int i = len - 1; i >= 0; i--) {
		result[len - i - 1] = str[i];
	}
	return result;
}

/*
Default constructor. Does nothing.
*/
GCodeParser::GCodeParser() {

}

/*
Parses the character array to a single GCode object and returns it.
*/
GCode* GCodeParser::parse(char input[]) {
	double temp = 0.0;
	unsigned long position = 1;
	bool gotVal = false; // got some value before letter
	bool parseString = false;
	char *str = new char[100];
	int j = 0;
	GCode *code = new GCode();

	for (char i = this->maxInputLength; i > -1; i--) {
	
		if (input[i] == '@') {
			parseString = !parseString;
		}
		else if (parseString) {
			str[j] = input[i];
			j++;
		}
		else if (input[i] >= '0' && input[i] <= '9') {
			temp += (input[i] - '0') * position;
			position *= 10;
			gotVal = true;
		}
		else if (input[i] == 'G') {
			gotVal ? code->setG(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'X') {
			gotVal ? code->setX(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'Y') {
			gotVal ? code->setY(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'Z') {
			gotVal ? code->setZ(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'I') {
			gotVal ? code->setI(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'J') {
			gotVal ? code->setJ(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'M') {
			gotVal ? code->setM(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'S') {
			gotVal ? code->setS(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'F') {
			gotVal ? code->setF(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'R') {
			gotVal ? code->setR(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'P') {
			gotVal ? code->setP(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'Q') {
			gotVal ? code->setQ(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'r') {
			gotVal ? code->set_r(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'g') {
			gotVal ? code->set_g(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == 'b') {
			gotVal ? code->set_b(temp) : 0;
			gotVal = false;
			temp = 0.0;
			position = 1;
		}
		else if (input[i] == '-') {
			temp *= -1;
		}
		else if (input[i] == '.') {
			temp /= position;
			position = 1;
		}
		else {
			temp = 0.0;
			position = 1;
			gotVal = false;
			parseString = false;
		}
	}

	code->setString(flip(str, j));

	for (int i = 0; i < this->maxInputLength; i++){
		input[i] = '\0';
	}

	delete str;
	return code;
}

