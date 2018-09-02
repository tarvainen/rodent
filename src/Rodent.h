#ifndef RODENT_H
#define RODENT_H

#define MM_IN_INCHES 0.0393700787
#define MODE_COUNT 3

// limit errors
#define IN_X_LIMIT_ERROR -1
#define IN_Y_LIMIT_ERROR -2
#define IN_Z_LIMIT_ERROR -3
#define TOO_MUCH_REQURSION -4

#include "StepperManager.h"
#include "ManualController.h"
#include "Spindle.h"
#include "GCode.h"
#include "ThreadManager.h"
#include "CoordinateSystem.h"
#include "Vector2.h"
#include "RGBLeds.h"
#include "Switch.h"
#include "Speaker.h"
#include "RodentStatus.h"
#include "EEPROMWriter.h"

class Rodent {
private: 
	bool ready;
	bool connected;
	bool inManualMode;
	StepperManager *stepperManager;
	Spindle* spindle;
	ThreadManager *threadManager;
	GCode *lastGCode;
	CoordinateSystem *machineCoordinateSystem;
	CoordinateSystem *workCoordinateSystem;
	CoordinateSystem *currentCoordinateSystem;
	CoordinateSystem *realCoordinateSystem;
	RGBLeds *lights;
	Switch *manualSwitch;
	Switch *lidSwitch;
	Switch *emergencyButton;
	Speaker *speaker;
	bool absoluteProgramming;
	bool hasToMoveSomewhere;
	int xStepsPerMM;
	int yStepsPerMM;
	int zStepsPerMM;
	double xStepLen;
	double yStepLen;
	double zStepLen;
	bool paused;
	bool moving;
	bool moveOrdered;
	bool requestOrdered;
	bool millimetricalProgramming;
	bool compulsoryCancel;
	bool programCompulsoryCancel;
	bool lidOpen;

	int buzzerPin;
	// limits
	int limitX;
	int limitY;
	int limitZ;
	bool xInLimit;
	bool yInLimit;
	bool zInLimit;
	bool confingAxises;
	bool xConfing;
	bool yConfing;
	bool zConfing;

	bool gotRealX;
	bool gotRealY;

	bool _reportMode;
	bool _reportLights;
	bool _reportLid;
	bool _reportSpindle;
	bool _reportSounds;
	
	// dwelling properties
	bool dwelling;

	// drill properties
	double drillingTravelHeight; // travel Z coordinate while drilling
	double drillDepth; // bottom Z coordinate of the hole
	bool pecking; // creates the hole with multiple passes
	double peckingValue; // depth to increase on each peck
	double dwellAfterDrill; // if drill with dwell, dwell this time on the bottom of the hole

	// home positions
	double homeX;
	double homeY;
	double homeZ;
	bool inLinearMode;
	bool inDrillingMode;
	bool inArcMode;
	int _mode;


	// private gcode-functions
	void dwell(double);
	void drill();
	void arc(double, double, double, double, bool);

	bool *modes[MODE_COUNT] = { &this->inLinearMode, &this->inDrillingMode, &this->inArcMode};
	void setMode(char);

	long _lastCodeRequested; // the time in millis when last gcode was requested

	int automaticCoordinateSystemReset();
	void handleError(int);
	void justRunThreads(long);

	int reqursiveMoveCalls;

public:
	Rodent();
	int move();
	void moveManual();
	bool isMoving() { return this->moving; }
	void isMoving(bool moving) { this->moving = moving; }
	void orderMove() { this->moveOrdered = true; }
	void orderMove(bool order) { this->moveOrdered = order; }
	void init();
	void attachStepperManager(StepperManager*);
	StepperManager* getStepperManager();
	void obey(GCode *code);
	void attachManualController(ManualController*);
	ManualController* getManualController();
	void attachSpindle(Spindle*);
	Spindle* getSpindle();
	void setBuzzerPin(int);
	void setLimits(int, int, int);
	void attachThreadManager(ThreadManager*);
	ThreadManager* getThreadManager();
	CoordinateSystem *getMachineCoordinateSystem() { return this->machineCoordinateSystem; }
	CoordinateSystem *getWorkCoordinateSystem() { return this->workCoordinateSystem; }
	CoordinateSystem *getCurrentCoordinateSystem() { return this->currentCoordinateSystem; }
	CoordinateSystem *getRealCoordinateSystem() { return this->realCoordinateSystem; }
	void setMachineCoordinateSystem(CoordinateSystem*);
	void setWorkCoordinateSystem(CoordinateSystem*);
	void setRealCoordinateSystem(CoordinateSystem*);
	void selectMachineCoordinateSystem() { this->currentCoordinateSystem = this->machineCoordinateSystem; }
	void selectWorkCoordinateSystem() { this->currentCoordinateSystem = this->workCoordinateSystem; }
	void setRGBLightSystem(RGBLeds *lights) { this->lights = lights; }
	RGBLeds *getRGBLightSystem() { return this->lights; }
	void setManualSwitch(Switch *s) { this->manualSwitch = s; }
	Switch *getManualSwitch() { return this->manualSwitch; }
	void setLidSwitch(Switch *s) { this->lidSwitch = s; }
	Switch *getLidSwitch() { return this->lidSwitch; }
	void setEmergencyButton(Switch *s) { this->emergencyButton = s; }
	Switch *getEmergencyButton() { return this->emergencyButton; }
	void setSpeaker(Speaker* speaker) { this->speaker = speaker; }
	Speaker *getSpeaker() { return this->speaker; }
	void setStatus(StatusObject);
	bool isOnAbsoluteProgramming() { return this->absoluteProgramming; }
	bool hasToMove(bool);
	bool hasToMove();
	bool isTimeToMove();
	void setPitches(double, double, double);
	int getXStepsPerMM() { return this->xStepsPerMM; }
	int getYStepsPerMM() { return this->yStepsPerMM; }
	int getZStepsPerMM() { return this->zStepsPerMM; }
	void pause();
	bool isPaused();
	void stop();
	void programStop();
	void programEnable();
	void mayStartAgain();
	bool isConnected() { return this->connected; }

	void setHomeX(double);
	void setHomeY(double);
	void setHomeZ(double);
	double getHomeX();
	double getHomeY();
	double getHomeZ();

	bool isProgrammingInMillimetres() { return this->millimetricalProgramming; }

	void setTargetCoordinates(GCode *code);
	void setTargetX(double);
	void setTargetY(double);
	void setTargetZ(double);

	void xIsLimiting(bool);
	void yIsLimiting(bool);
	void zIsLimiting(bool);

	void setManualState(bool);
	bool isInManualMode() { return this->inManualMode; }
	void setLidState(bool);
	void setEmergencyButtonState(bool);
	void setUnitMode(bool);
	void setRelativeMode(bool);

	// serial message functions
	void requestNewCoordinate();
	void sayHelloToSerial();
	double requestRealCoordinatesX();
	double requestRealCoordinatesY();


	void reportLights();
	void reportSounds();
	void reportLid();
	void reportSpindle();
	void reportMode();
	void reportError(double);
	void reportManualState();
	void reportUnitMode();
	void reportRelativeMode();
	void reportStopMode();

	void playAlertSound();

	void finallyGotRealX() { this->gotRealX = true; }
	void finallyGotRealY() { this->gotRealY = true; }
	void clearEncoder();

	void sendSSID(char*);
	void sendPassword(char*);
	void connectWiFi();
};

#endif