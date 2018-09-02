#ifndef RODENT_STATUS_H
#define RODENT_STATUS_H
#include "CoordinateSystem.h"

struct StatusObject {
	int red = 0;
	int green = 0;
	int blue = 0;
	bool lightsOn = false;
	double machineX = 0.0;
	double machineY = 0.0;
	double machineZ = 0.0;
};

class RodentStatus {
private:
	// machine coordinates
	CoordinateSystem *machineCoordinateSystem;
	bool _absoluteProgramming;
	bool _lightsOn;
	int _red;
	int _green;
	int _blue;
	bool _soundsOn;
	void init();
public:
	RodentStatus();
	void setCoordinateSystem(CoordinateSystem*);
	CoordinateSystem *getCoordinateSystem();
	void setAbsoluteProgrammingState(bool);
	bool getAbsoluteProgrammingState();
	void setLightsState(bool);
	bool getLightState();
	void setRedValue(int);
	int getRedValue();
	void setGreenValue(int);
	int getGreenValue();
	void setBlueValue(int);
	int getBlueValue();
	void setSoundState(bool);
	bool getSoundState();
	StatusObject status;

	
};

#endif