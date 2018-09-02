#include "Rodent.h"
#include "Arduino.h"
#include "sounds.h"
#define INCHES_IN_MM 25.4

#define LINEAR_MODE 0
#define DRILLING_MODE 1
#define ARC_MODE 2


/*
Default constructor.
*/
Rodent::Rodent() {
	this->init();
}

/*
Initializes all the components to their default values.
*/
void Rodent::init() {
	this->ready = false;
	this->inManualMode = false;
	this->stepperManager = new StepperManager();
	this->spindle = new Spindle();
	this->threadManager = new ThreadManager();
	this->machineCoordinateSystem = new CoordinateSystem();
	this->workCoordinateSystem = new CoordinateSystem();
	this->currentCoordinateSystem = new CoordinateSystem();
	this->realCoordinateSystem = new CoordinateSystem();
	this->currentCoordinateSystem = this->machineCoordinateSystem;
	this->hasToMoveSomewhere = false;
	this->paused = false;
	this->moving = false;
	this->moveOrdered = false;
	this->requestOrdered = false;
	this->homeX = 0.0;
	this->homeY = 0.0;
	this->homeZ = 0.0;
	this->millimetricalProgramming = true;
	this->setMode(LINEAR_MODE);
	this->absoluteProgramming = true;
	this->compulsoryCancel = false;
	this->programCompulsoryCancel = false;
	this->connected = false;
	this->lidOpen = false;

	this->buzzerPin = -1;
	
	// limits
	this->limitX = -1;
	this->limitY = -1;
	this->limitZ = -1;
	this->xInLimit = false;
	this->yInLimit = false;
	this->zInLimit = false;
	this->xConfing = false;
	this->yConfing = false;
	this->zConfing = false;
	this->confingAxises = false;

	// reporting flags (turned to true when has to be reported)
	this->_reportLights = false;
	this->_reportLid = false;
	this->_reportMode = false;
	this->_reportSounds = false;
	this->_reportSpindle = false;

	// dwelling params
	this->dwelling = false;

	// drilling params
	this->drillDepth = 0.0;
	this->drillingTravelHeight = 0.0;
	this->pecking = false;
	this->peckingValue = 0.0;

	this->gotRealX = false;
	this->_lastCodeRequested = 0;
	this->reqursiveMoveCalls = 0;

}

/*
Attaches stepper manager.
*/
void Rodent::attachStepperManager(StepperManager *manager) {
	this->stepperManager = manager;
}

/*
Attaches spindle.
*/
void Rodent::attachSpindle(Spindle *spindle) {
	this->spindle = spindle;
}

/*
Sets limit pins.
*/
void Rodent::setLimits(int x, int y, int z) {
	this->limitX = x;
	this->limitY = y;
	this->limitZ = z;

	// set pins to input mode
	pinMode(x, INPUT);
	pinMode(y, INPUT);
	pinMode(z, INPUT);
	
	// turn on pullup resistors
	digitalWrite(x, HIGH);
	digitalWrite(y, HIGH);
	digitalWrite(z, HIGH);
}

/*
Do things depending on the code got from serial port.
*/
void Rodent::obey(GCode *code) {
	StatusObject status;

	this->lastGCode = code;
	if (code->isG()) { // is gcode
		int g = (int)code->getG();
		switch (g) {
		case 0: // rapid movement
			// movement will be created on the other if statement below
			// set motor speed due the maximum feedrate
			this->stepperManager->setDelayBetweenSteps(this->stepperManager->getMinimumDelayBetweenSteps());
			this->setMode(LINEAR_MODE);
			break;
		case 1: // linear interpolation
			// movement will be created on the other if statement below
			// turn move flag up and enable motors
			this->setMode(LINEAR_MODE);
			break;
		case 2:
			// circular interpolation (clockwise)
			if (code->isX() && code->isY() && code->isJ() && code->isI()) {
				this->setMode(ARC_MODE);
				this->stepperManager->enableAll();
				this->arc(code->getI(), code->getJ(), code->getX(), code->getY(), true);
			}
			break;
		case 3:
			// circular interpolation (counter-clockwise)
			if (code->isX() && code->isY() && code->isJ() && code->isI()) {
				this->setMode(ARC_MODE);
				this->stepperManager->enableAll();
				this->arc(code->getI(), code->getJ(), code->getX(), code->getY(), false);
			}
			break;
		case 4:
			// dwell
			if (code->isP()) {
				this->dwell(code->getP());
			}
			break;
		case 5:
			// high precision contour control
			break;
		case 6:
			// non-uniform rational b-spline
			break;
		case 9:
			// exact stop
			break;
		case 10:
			// programmable data input
			break;
		case 11:
			// data write cancel
			this->programStop();
			break;
		case 12:
			// full-circle interpolation (clockwise)
			break;
		case 13:
			// full-cicrle interpolation (counter-clockwise)
			break;
		case 17:
			// XY plane selection
			break;
		case 18:
			// ZX plane selection
			break;
		case 19:
			// YZ plane selection
			break;
		case 20:
			// programming in inches
			this->setUnitMode(false);
			this->currentCoordinateSystem->setToInches();
			break;
		case 21:
			// programming in mm
			this->setUnitMode(true);
			this->currentCoordinateSystem->setToMM();
			break;
		case 28:
			// return to home position
			this->currentCoordinateSystem->setTargetX(this->homeX);
			this->currentCoordinateSystem->setTargetY(this->homeY);
			this->currentCoordinateSystem->setTargetZ(this->homeZ);
			this->hasToMoveSomewhere = true;
			this->stepperManager->enableAll();
			break;
		case 30:
			// return to secondary home position
			break;
		case 31:
			// skip function
			break;
		case 40:
			// tool radius compensation off
			break;
		case 41:
			// tool radius compensation left
			break;
		case 42:
			// tool radius compensation right
			break;
		case 43:
			// tool height offset compensation negative
			break;
		case 44:
			// tool height offset compensation positive
			break;
		case 45:
			// axis offset single increase
			break;
		case 46:
			// axis offset single decrease
			break;
		case 47:
			// axis offset double increase
			break;
		case 48:
			// axis offset double decrease
			break;
		case 49:
			// tool length offset compensation cancel
			break;
		case 50:
			// scaling function cancel
			break;
		case 52:
			// local coordinate system
			break;
		case 53:
			// machine coordinate system
			this->currentCoordinateSystem = this->machineCoordinateSystem;
			this->realCoordinateSystem->setCurrentX(this->currentCoordinateSystem->getCurrentX());
			this->realCoordinateSystem->setCurrentY(this->currentCoordinateSystem->getCurrentY());
			this->realCoordinateSystem->setCurrentZ(this->currentCoordinateSystem->getCurrentZ());
			this->realCoordinateSystem->makeAllEqual();
			break;
		case 54:
			// work coordinate system
			this->currentCoordinateSystem = this->workCoordinateSystem;
			this->realCoordinateSystem->setCurrentX(this->currentCoordinateSystem->getCurrentX());
			this->realCoordinateSystem->setCurrentY(this->currentCoordinateSystem->getCurrentY());
			this->realCoordinateSystem->setCurrentZ(this->currentCoordinateSystem->getCurrentZ());
			this->realCoordinateSystem->makeAllEqual();
			break;
		case 61:
			// exact stop check
			break;
		case 62:
			// automatic corner override
			break;
		case 64:
			// default cutting mode
			break;
		case 70:
			// fixed cycle, multiple cycle for finishing
			break;
		case 71:
			// fixed cycle, multiple cycle for roughing (z-axis emphasis)
			break;
		case 72:
			// fixed cycle, multiple cycle for roughing (x-axis emphasis)
			break;
		case 73:
			// fixed cycle, multiple cycle for shapes
			break;
		case 74:
			// peck drilling
			break;
		case 75:
			// peck grooving cycle for turning
			break;
		case 76:
			// fine boring cycle for milling
			break;
		case 80:
			// cancel canned cycle
			break;
		case 81:
			// simple drilling cycle
			break;
		case 82:
			// drilling cycle with dwell
			this->setMode(DRILLING_MODE);
			this->pecking = false;

			if (code->isP()) {
				this->dwellAfterDrill = code->getP();
			}
			if (code->isR()) {
				this->drillingTravelHeight = code->getR();
			}
			if (code->isZ()) {
				this->drillDepth = code->getZ();
			}

			break;
		case 83:
			// peck drilling cycle
			break;
		case 90:
			// absolute programming
			this->setRelativeMode(false);
			break;
		case 91:
			// relative programming
			this->setRelativeMode(true);
			break;
		case 92:
			// position register
			break;
		case 94:
			// feedrate per minute
			break;
		case 95:
			// feedrate per revolution
			break;
		default:
			break;
		}
	}

	if (code->isM() && !code->isG() && !code->isR()) {
		int m = (int)code->getM();
		switch (m) {
		case 0:
			// compulsory stop
			this->programStop();
			break;
		case 1:
			// optional stop
			this->pause();
			break;
		case 2:
			// end of program
			this->currentCoordinateSystem->setTargetX(this->homeX);
			this->currentCoordinateSystem->setTargetY(this->homeY);
			this->currentCoordinateSystem->setTargetZ(this->homeZ);
			this->hasToMoveSomewhere = true;
			this->stepperManager->enableAll();
			this->move();
			break;
		case 3:
			// spindle on (clockwise)
			this->spindle->spin();
			this->reportSpindle();
			break;
		case 4:
			// spindle on (counterclockwise)
			break;
		case 5:
			// spindle stop
			this->spindle->stop();
			this->reportSpindle();
			break;
		case 6:
			// automatic tool change
			break;
		case 7:
			// coolant on (mist)
			break;
		case 8:
			// coolant on (flood)
			break;
		case 9:
			// coolant off
			break;
		case 13:
			// spindle on (clockwise) and coolant on (flood)
			break;
		case 19:
			// spindle orientation
			break;
		case 21:
			this->currentCoordinateSystem->mirrorX(true);
			break;
		case 22:
			this->currentCoordinateSystem->mirrorY(true);
			// mirror y-axis
			break;
		case 23:
			this->currentCoordinateSystem->mirrorX(false);
			this->currentCoordinateSystem->mirrorY(false);
			// mirror off
			break;
		case 24: // make all work coordinates zero
			this->workCoordinateSystem->setZero();
			break;
		case 25: // make work x coordinate zero
			this->workCoordinateSystem->setZeroX();
			break;
		case 26: // make work y coordinate zero
			this->workCoordinateSystem->setZeroY();
			break;
		case 27: // make work z coordinate zero
			this->workCoordinateSystem->setZeroZ();
			break;
		case 28: // make all machine coordinates zero
			this->machineCoordinateSystem->setZero();
			this->realCoordinateSystem->setZero();
			this->clearEncoder();
			break;
		case 30:
			// program end, return to program top
			this->currentCoordinateSystem->setTargetX(this->homeX);
			this->currentCoordinateSystem->setTargetY(this->homeY);
			this->currentCoordinateSystem->setTargetZ(this->homeZ);
			this->hasToMoveSomewhere = true;
			this->stepperManager->enableAll();
			break;
		case 31: // program stop disable (enable)
			this->programEnable();
			break;
		case 50:
			// auto machine coordinate system reset (zero center in every axis)
			this->automaticCoordinateSystemReset();
		default:
			break;
		}

	}

	// gets something about feedrate in the gcode
	if (code->isF()) {

	}

	// gets something about rodent in the gcode
	if (code->isR() && !code->isG()) {
		switch ((int)code->getR()) {
		case 0: // "Hi, I am the Rodent."
			break;
		case 1: // "Hi, I am the Rodent controlling software."
			this->connected = true;
			this->reportLights();
			this->reportLid();
			this->reportMode();
			this->reportSounds();
			this->reportSpindle();
			this->reportManualState();
			this->reportRelativeMode();
			this->reportUnitMode();
			this->reportStopMode();
			this->speaker->play(new Sound(*LOGIN_SOUND));
			return;
			break;
		case 2: // "Hi, would you like to answer me?"
			this->connected = false;
			break;
		case 6: // Got some led values for ya!
			if (code->is_r()) {
				this->lights->setRedValue(code->get_r());
			}
			if (code->is_g()) {
				this->lights->setGreenValue(code->get_g());
			}
			if (code->is_b()) {
				this->lights->setBlueValue(code->get_b());
			}
			this->reportLights();
			break;
		case 9: // rodent software asks am I ready
			if (!this->paused) {
				this->requestNewCoordinate();
				return;
			}
			break;
		case 10: // request new line, actually do nothing here
			break;
		case 11: // request last message again
			break;
		case 15: // mute speaker
			this->speaker->mute();
			this->reportSounds();
			break;
		case 16: // mute speaker
			this->speaker->unmute();
			this->reportSounds();
			break;
		case 20: // lights on !!
			this->lights->on();
			this->reportLights();
			break;
		case 21: // lights off !!
			this->lights->off();
			this->reportLights();
			break;
		case 22:
			this->lights->setRedValue(code->get_r());
			break;
		case 23:
			this->lights->setGreenValue(code->get_g());
			break;
		case 24:
			this->lights->setBlueValue(code->get_b());
			break;
		case 27: // save situation
			status.red = this->lights->getRedValue();
			status.green = this->lights->getGreenValue();
			status.blue = this->lights->getBlueValue();
			status.lightsOn = this->lights->isOn();
			status.machineX = this->machineCoordinateSystem->getCurrentX();
			status.machineY = this->machineCoordinateSystem->getCurrentY();
			status.machineZ = this->machineCoordinateSystem->getCurrentZ();
			EEPROMWriter::write(status);
			this->speaker->play(new Sound(*LOGOUT_SOUND));
			break;
		case 29:
			this->spindle->setPWM(code->getQ());
			break;
		case 90:
			//if (code->is_str()) {
			//	this->sendSSID(code->getString());
			//}
			break;
		case 91:
			//if (code->is_str()) {
			//	this->sendPassword(code->getString());
			//}
			break;
		case 92:
			this->connectWiFi();
			break;
		case 100: // party mode !!!
			this->speaker->play(new Sound(*SUPER_MARIO_SONG));
			break;

		case 150: // wifi connection established
			this->connected = true;
			reportSpindle();
			reportLights();
			this->speaker->play(new Sound(*CONNECTION_ESTABLISHED));
			break;
		case 151: // wifi connection closed
			this->speaker->play(new Sound(*CONNECTION_CLOSED));
			break;
		default:
			break;
		}
	}

	// gets some coordinater
	if (code->isX() || code->isY() || code->isZ()) {
		if (this->inLinearMode) { // linear interpolation
			this->setTargetCoordinates(code);
			this->hasToMoveSomewhere = true;
			this->stepperManager->enableAll();
			this->move();
		}
		else if (this->inDrillingMode && code->isX() && code->isY()) { // drilling something
			this->hasToMoveSomewhere = true;
			this->stepperManager->enableAll();
			this->currentCoordinateSystem->setTargetX(code->getX());
			this->setTargetX(code->getX());
			this->setTargetY(code->getY());
			this->move(); // move to the drill position
			this->drill(); // drill hole or spot
		}
		
	}
	else {
	}

	if (code->isQ()) {
		switch ((int)code->getQ()) {
		case 0: // request new coordinate
			this->requestNewCoordinate();
			break;
		default:
			break;
		}
	}

}

/*
Sets the target coordinates to the current coordinate system due the Rodent absolute or relative state.
*/
void Rodent::setTargetCoordinates(GCode *code) {
	if (code->isX()) { // got x value from the gcode
		this->setTargetX(code->getX());
	}
	if (code->isY()) { // got y value from the gcode
		this->setTargetY(code->getY());
	}
	if (code->isZ()) { // got z value from the gcode
		this->setTargetZ(code->getZ());
	}
}

/*
Sets the target X coordinate to the current coordinate system due the Rodent's absolute or relative state.
*/
void Rodent::setTargetX(double x) {
	if (this->absoluteProgramming) {
		this->currentCoordinateSystem->setTargetX(x);
	}
	else {
		this->currentCoordinateSystem->setTargetX(this->currentCoordinateSystem->getCurrentX() + x);
	}
}

/*
Sets the target Y coordinate to the current coordinate system due the Rodent's absolute or relative state.
*/
void Rodent::setTargetY(double y) {
	if (this->absoluteProgramming) {
		this->currentCoordinateSystem->setTargetY(y);
	}
	else {
		this->currentCoordinateSystem->setTargetY(this->currentCoordinateSystem->getCurrentY() + y);
	}
}

/*
Sets the target Z coordinate to the current coordinate system due the Rodent's absolute or relative state.
*/
void Rodent::setTargetZ(double z) {
	if (this->absoluteProgramming) {
		this->currentCoordinateSystem->setTargetZ(z);
	}
	else {
		this->currentCoordinateSystem->setTargetZ(this->currentCoordinateSystem->getCurrentZ() + z);
	}
}

/*
Attaches a thread manager to the Rodent.
*/
void Rodent::attachThreadManager(ThreadManager *manager) {
	this->threadManager = manager;
}

/*
Returns Rodent's thread manager.
*/
ThreadManager* Rodent::getThreadManager() {
	return this->threadManager;
}

/*
Returns Rodent's stepper manager.
*/
StepperManager* Rodent::getStepperManager() {
	return this->stepperManager;
}

/*
Steps Rodent's stepper motors if they have to step after all.
*/
int Rodent::move() {
	this->moving = true;
	// timestamp for faster step
	long startTime = -1;
	// value to multiply with if programming in inches
	double inchFactor = 1;
	if (!this->millimetricalProgramming) {
		inchFactor = INCHES_IN_MM;
	}

	// count step amounts
	long xSteps = (this->currentCoordinateSystem->getTargetX() - this->currentCoordinateSystem->getCurrentX()) * this->getXStepsPerMM() * inchFactor;
	long ySteps = (this->currentCoordinateSystem->getTargetY() - this->currentCoordinateSystem->getCurrentY()) * this->getYStepsPerMM() * inchFactor;
	long zSteps = (this->currentCoordinateSystem->getTargetZ() - this->currentCoordinateSystem->getCurrentZ()) * this->getZStepsPerMM() * inchFactor;

	double targetX = this->currentCoordinateSystem->getTargetX();
	double targetY = this->currentCoordinateSystem->getTargetY();
	double targetZ = this->currentCoordinateSystem->getTargetZ();

	// direction variables (either -1 or 1)
	int dirX = 1;
	int dirY = 1;
	int dirZ = 1;
	xSteps < 0 ? dirX = -1 : dirX = 1;
	ySteps < 0 ? dirY = -1 : dirY = 1;
	zSteps < 0 ? dirZ = -1 : dirZ = 1;

	// sets directions to the stepper manager
	this->stepperManager->setDirections(dirX, dirY, dirZ);

	// assume the X is the biggest...
	long biggest = xSteps * dirX;
	double incrementX = 1.0;
	double incrementY = (double)(ySteps * dirY) / (xSteps * dirX);
	double incrementZ = (double)(zSteps * dirZ) / (xSteps * dirX);
	// ... but if it's not...
	if (xSteps * dirX < ySteps * dirY) {
		biggest = ySteps * dirY;
		incrementX = (double)(xSteps * dirX) / (ySteps * dirY);
		incrementZ = (double)(zSteps * dirZ) / (ySteps * dirY);
		incrementY = 1.0;
		// ...is it Z then? Or...
		if (zSteps * dirZ > ySteps * dirY) {
			biggest = zSteps * dirZ;
			incrementX = (double)(xSteps * dirX) / (zSteps * dirZ);
			incrementY = (double)(ySteps * dirY) / (zSteps * dirZ);
			incrementZ = 1.0;
		}
	}
	else {
		// ...yes, it has to be Z.
		if (zSteps * dirZ > xSteps * dirX) {
			biggest = zSteps * dirZ;
			incrementX = (double)(xSteps * dirX) / (zSteps * dirZ);
			incrementY = (double)(ySteps * dirY) / (zSteps * dirZ);
			incrementZ = 1.0;
		}
	}

	// vars for saving last values (increment values, not real coordinates)
	double lastValX = 0.0;
	double lastValY = 0.0;
	double lastValZ = 0.0;
	// these are the real coordinates
	double xCoordMachine = this->machineCoordinateSystem->getCurrentX();
	double yCoordMachine = this->machineCoordinateSystem->getCurrentY();
	double zCoordMachine = this->machineCoordinateSystem->getCurrentZ();

	double xCoordWork = this->workCoordinateSystem->getCurrentX();
	double yCoordWork = this->workCoordinateSystem->getCurrentY();
	double zCoordWork = this->workCoordinateSystem->getCurrentZ();

	for (long i = 0; i < biggest; i++) {
		startTime = micros();
		if ((int)(lastValX + incrementX + 0.5) > (int)(lastValX + 0.5) && xSteps != 0) { // if it's time to step X
			this->stepperManager->getX()->step();
			double inX = (double)dirX / this->getXStepsPerMM() * inchFactor;
			xCoordMachine += inX;
			xCoordWork += inX;
			this->getMachineCoordinateSystem()->setCurrentX(xCoordMachine);
			if (this->currentCoordinateSystem == this->workCoordinateSystem) {
				this->workCoordinateSystem->setCurrentX(xCoordWork);
			}
		}

		if ((int)(lastValY + incrementY + 0.5) > (int)(lastValY + 0.5) && ySteps != 0) { // and maybe we step Y too?
			this->stepperManager->getY()->step();
			double inY = (double)dirY / this->getYStepsPerMM() * inchFactor;
			yCoordMachine += inY;
			yCoordWork += inY;
			this->getMachineCoordinateSystem()->setCurrentY(yCoordMachine);
			if (this->currentCoordinateSystem == this->workCoordinateSystem) {
				this->workCoordinateSystem->setCurrentY(yCoordWork);
			}
		}

		if ((int)(lastValZ + incrementZ + 0.5) > (int)(lastValZ + 0.5) && zSteps != 0) { // we must not forget the Z.
			this->stepperManager->getZ()->step();
			double inZ = (double)dirZ / this->getZStepsPerMM() * inchFactor;
			zCoordMachine += inZ;
			zCoordWork += inZ;
			this->getMachineCoordinateSystem()->setCurrentZ(zCoordMachine);
			if (this->currentCoordinateSystem == this->workCoordinateSystem) {
				this->workCoordinateSystem->setCurrentZ(zCoordWork);
			}
		}
		// wait for it...do something else while waiting (everything added to thread manager will be executed)
		this->justRunThreads(this->stepperManager->getDelayBetweenSteps() - (micros() - startTime));

		// add increments to last value containers
		lastValX += incrementX;
		lastValY += incrementY;
		lastValZ += incrementZ;
		if (this->compulsoryCancel || this->programCompulsoryCancel) {
			return 0;
		}
	}

	if (!this->inManualMode) {
		bool hasMove = false;
		if (abs(this->realCoordinateSystem->getCurrentX() - this->machineCoordinateSystem->getCurrentX()) > 0.01) {
			double tX = this->requestRealCoordinatesX();
			if (this->gotRealX) {
				this->gotRealX = false;
				this->machineCoordinateSystem->setCurrentX(tX);
				double diffX = this->machineCoordinateSystem->getTargetX() - tX;
				this->setTargetX(tX + diffX);
				hasMove = true;
			}
		}

		if (abs(this->realCoordinateSystem->getCurrentY() - this->machineCoordinateSystem->getCurrentY()) > 0.01) {
			double tY = this->requestRealCoordinatesY();
			if (this->gotRealY) {
				this->gotRealY = false;
				this->machineCoordinateSystem->setCurrentY(tY);
				double diffY = this->machineCoordinateSystem->getTargetY() - tY;
				this->setTargetY(tY + diffY);
				hasMove = true;
			}
		}

		if (hasMove && this->reqursiveMoveCalls <= 2) {
			this->reqursiveMoveCalls++;
			this->move();
		}
		else if (this->reqursiveMoveCalls > 2) {
			this->reportError(TOO_MUCH_REQURSION);
			this->clearEncoder();
			this->hasToMove(false);
			this->workCoordinateSystem->makeAllEqual();
			this->machineCoordinateSystem->makeAllEqual();
		} 
		else {
			this->clearEncoder();
			this->hasToMove(false);
			this->workCoordinateSystem->makeAllEqual();
			this->machineCoordinateSystem->makeAllEqual();
		}
	}
	this->stepperManager->setTimeout(millis());
	this->moving = false;

	this->reqursiveMoveCalls = 0;
	if (this->moving) {
		return -1;
	}
	else {
		return 0;
	}

}

/*
Drills hole to the current XY position to the depth specified by drilling depth property.
*/
void Rodent::drill() {
	if (!this->inDrillingMode) { return; }
	this->setTargetZ(this->drillDepth);
	this->move();
	if (this->dwellAfterDrill > 0) {
		this->dwell(dwellAfterDrill);
	}
	this->setTargetZ(this->drillingTravelHeight);
	this->move();
}

/*
Creates an arc with the values given. Creates three vectors (start, end and rotation vector) and 
counts target positions out of rotation vector's end point.
*/
void Rodent::arc(double centerX, double centerY, double endX, double endY, bool clockwise) {
	double startX = this->currentCoordinateSystem->getCurrentX(); // start X coordinate (current X)
	double startY = this->currentCoordinateSystem->getCurrentY(); // start Y coordinate (current Y)
	Vector2 *startVector = new Vector2(centerX, centerY, startX, startY, true); // start vector
	Vector2 *endVector = new Vector2(centerX, centerY, endX, endY, true); // end vector

	double angleToRotate = 0.0;
	char wisefactor = 0; // 1 if clockwise, -1 if counterclockwise, multiplies the increment (can make it decrement)

	if (clockwise) {
		if (startVector->direction() > endVector->direction()) {
			angleToRotate = startVector->direction() - endVector->direction();
		}
		else if (startVector->direction() < endVector->direction()) {
			angleToRotate = startVector->direction() - (endVector->direction() - 2 * PI);
		}
		else if (startVector->direction() == endVector->direction()) {
			angleToRotate = 2 * PI; // full circle
		}
		wisefactor = -1; // clockwise
	}
	else {
		if (startVector->direction() > endVector->direction()) {
			angleToRotate = (2 * PI - startVector->direction()) + endVector->direction();
		}
		else if (startVector->direction() < endVector->direction()) {
			angleToRotate = endVector->direction() - startVector->direction();
		}
		else if (startVector->direction() == endVector->direction()) {
			angleToRotate = 2 * PI; // full circle
		}
		wisefactor = 1; // counterclockwise
	}

	int stepsPerRotation = 360; // how many pieces will the full circle break into
	double partOfCircle = angleToRotate / (2 * PI);
	int steps = partOfCircle * stepsPerRotation;
	double increment = (2 * PI) / stepsPerRotation;

	Vector2 *rotateVector = new Vector2(centerX, centerY, startX, startY, true); // vector to rotate

	// rotate the rotation vector, count target positions and move Rodent
	for (int i = 0; i < steps; i++) {
		rotateVector->direction(rotateVector->direction() + (wisefactor * increment));
		double dX = rotateVector->endX() - this->currentCoordinateSystem->getCurrentX();
		double dY = rotateVector->endY() - this->currentCoordinateSystem->getCurrentY();
		this->currentCoordinateSystem->setTargetX(rotateVector->endX());
		this->currentCoordinateSystem->setTargetY(rotateVector->endY());
		dX *= this->xStepsPerMM;
		dY *= this->yStepsPerMM;
		this->hasToMoveSomewhere = true;
		this->move();
	}

	// delete vector pointers
	startVector = NULL;
	delete startVector;
	endVector = NULL;
	delete endVector;
	rotateVector = NULL;
	delete rotateVector;

	// sets current values to given target ones
	this->currentCoordinateSystem->setCurrentX(endX);
	this->currentCoordinateSystem->setCurrentY(endY);
	this->currentCoordinateSystem->makeAllEqual();
}

/*
Sets the machine mode for example to the linear mode or the drilling mode.
*/
void Rodent::setMode(char mode) {
	for (int i = 0; i < MODE_COUNT; i++) {
		*this->modes[i] = false;
	}

	*this->modes[mode] = true;
	this->_mode = mode;
	this->reportMode();
}

/*
Returns true if Rodent has to move somewhere.
*/
bool Rodent::hasToMove() {
	return this->hasToMoveSomewhere;
}

/*
Says to the Rodent if he has to move somewhere.
*/
bool Rodent::hasToMove(bool hasToMove) {
	this->hasToMoveSomewhere = hasToMove;
	return hasToMove;
}

/*
Returns true if it's time to move the Rodent.
*/
bool Rodent::isTimeToMove() {
	long dur = micros() - this->stepperManager->getLastStepTime();
	if (dur >= this->stepperManager->getDelayBetweenSteps()) {
		return true;
	}
	else {
		return false;
	}
}

/*
Sets the steps per mm values and step lengths.
*/
void Rodent::setPitches(double x, double y, double z) {
	this->xStepsPerMM = this->stepperManager->getX()->getStepsPerRevolution() / x;
	this->xStepLen = x / this->stepperManager->getX()->getStepsPerRevolution();
	this->yStepsPerMM = this->stepperManager->getY()->getStepsPerRevolution() / y;
	this->yStepLen = y / this->stepperManager->getY()->getStepsPerRevolution();
	this->zStepsPerMM = this->stepperManager->getZ()->getStepsPerRevolution() / z;
	this->zStepLen = z / this->stepperManager->getZ()->getStepsPerRevolution();
}

/*
Compulsorely stops the program.
*/
void Rodent::stop() {
	this->hasToMove(false);
	this->workCoordinateSystem->makeAllEqual();
	this->machineCoordinateSystem->makeAllEqual();
	this->stepperManager->disableAll();
	this->compulsoryCancel = true;
	this->moving = false;
	this->reportStopMode();
}

/*
Optionally pauses the program. Calling the same function again will unpause the program.
*/
void Rodent::pause() {
	this->paused = !this->paused;
	if (paused) {
		this->stepperManager->disableAll();
	}
	else {
		this->stepperManager->enableAll();
	}
}

/*
Returns true if the Rodent is paused.
*/
bool Rodent::isPaused() {
	return this->paused;
}

/*
Sets the home's X coordinate.
*/
void Rodent::setHomeX(double coord) {
	this->homeX = coord;
}

/*
Returns the home's X coordinate.
*/
double Rodent::getHomeX() {
	return this->homeX;
}

/*
Sets the home's Y coordinate.
*/
void Rodent::setHomeY(double coord) {
	this->homeY = coord;
}

/*s
Returns the home's Y coordinate.
*/
double Rodent::getHomeY() {
	return this->homeY;
}

/*
Sets the home's Z coordinate.
*/
void Rodent::setHomeZ(double coord) {
	this->homeZ = coord;
}

/*
Returns the home's Z coordinate.
*/
double Rodent::getHomeZ() {
	return this->homeZ;
}

/*
Requests new coordinate from the controlling software.
*/
void Rodent::requestNewCoordinate() {
	if (!this->compulsoryCancel) { // if data writing has not been canceled
		Serial.println("R10");
		Serial1.println("R10");
		this->_lastCodeRequested = millis();
		this->requestOrdered = false;
	}
}

/*
Says "Hi, I am the Rodent" to the serial. May have a new friend after that.
*/
void Rodent::sayHelloToSerial() {
	Serial.println("R0");
}

/*
Dwells given time in current position (seconds).
*/
void Rodent::dwell(double time) {
	long start = millis();
	long duration = 1000 * time;
	while (millis() - start <= duration) {
		this->threadManager->runThreads();
	}
}

/*
Sets X limit flag.
*/
void Rodent::xIsLimiting(bool limiting) {
	this->xInLimit = limiting;
	if (!this->confingAxises) {
		this->handleError(IN_X_LIMIT_ERROR);
	}
}

/*
Sets Y limit flag.
*/
void Rodent::yIsLimiting(bool limiting) {
	this->yInLimit = limiting;
	if (!this->confingAxises) {
		this->handleError(IN_Y_LIMIT_ERROR);
	}
}

/*
Sets Z limit flag.
*/
void Rodent::zIsLimiting(bool limiting) {
	this->zInLimit = limiting;
	if (!this->confingAxises) {
		this->handleError(IN_Y_LIMIT_ERROR);
	}
}

/*
Resets machine coordinate system (zero center on every axis).
*/
int Rodent::automaticCoordinateSystemReset() {
	this->currentCoordinateSystem = this->machineCoordinateSystem;

	// return an error if already in some limit (doesn't really know in which one)
	if (this->xInLimit || this->yInLimit || this->zInLimit) {
		return -1;
	}

	// axis lengths
	double xLen = 0.0;
	double yLen = 0.0;
	double zLen = 0.0;

	// stepped steps (amounts of steps, not distances)
	long steppedX = 0;
	long steppedY = 0;
	long steppedZ = 0;

	// bool flags up
	this->xConfing = true;
	this->yConfing = true;
	this->zConfing = true;
	this->confingAxises = true;

	// bool flags for first limit
	bool gotXFirstLimit = false;
	bool gotYFirstLimit = false;
	bool gotZFirstLimit = false;

	// bool flags for axis completions
	bool xDone = false;
	bool yDone = false;
	bool zDone = false;

	// flags for check if limit already checked, prevents stucking on limit
	bool xPull = false;
	bool yPull = false;
	bool zPull = false;
	
	// direction values
	int dirX = -1;
	int dirY = -1;
	int dirZ = -1;

	this->stepperManager->setDirections(dirX, dirY, dirZ);
	this->stepperManager->enableAll();
	long startMicros = 0; // timestamp for comparing and get fasted motor run

	// do something while confing one or more of the machine's axis
	while (!xDone || !yDone || !zDone) {
		startMicros = micros();

		if (this->xConfing) { // X axis code
			this->stepperManager->getX()->step();
			this->currentCoordinateSystem->setCurrentX(this->currentCoordinateSystem->getCurrentX() + dirX * this->xStepLen);

			if (!this->xInLimit) {
				xPull = false;
			}
			else if (this->xInLimit && !gotXFirstLimit && !xPull) {
				xPull = true; // limit checked
				gotXFirstLimit = true; // got left limit
				this->currentCoordinateSystem->setCurrentX(0.0);
				dirX *= -1;
				this->stepperManager->getX()->setDirection(dirX);
			}
			else if (this->xInLimit && gotXFirstLimit && !xPull) {
				this->xConfing = false;
				dirX *= -1;
				this->stepperManager->getX()->setDirection(dirX);
				xLen = this->currentCoordinateSystem->getCurrentX() / 2.0;
			}
		}
		else { // moves X to center
			if (steppedX < xLen / this->xStepLen) {
				this->stepperManager->getX()->step();
				this->currentCoordinateSystem->setCurrentX(this->currentCoordinateSystem->getCurrentX() + dirX * this->xStepLen);
				steppedX++;
			}
			else {
				this->stepperManager->getX()->disable();
				xDone = true;
			}
		}

		if (this->yConfing) { // Y axis code
			this->stepperManager->getY()->step();
			this->currentCoordinateSystem->setCurrentY(this->currentCoordinateSystem->getCurrentY() + dirY * this->yStepLen);

			if (!this->yInLimit) {
				yPull = false;
			}
			else if (this->yInLimit && !gotYFirstLimit && !yPull) {
				yPull = true;
				gotYFirstLimit = true;
				this->currentCoordinateSystem->setCurrentY(0.0);
				dirY *= -1;
				this->stepperManager->getY()->setDirection(dirY);
			}
			else if (this->yInLimit && gotYFirstLimit && !yPull) {
				this->yConfing = false;
				dirY *= -1;
				this->stepperManager->getY()->setDirection(dirY);
				yLen = this->currentCoordinateSystem->getCurrentY() / 2.0;
			}
		}
		else { // moves Y to center
			if (steppedY < yLen / this->yStepLen) {
				this->stepperManager->getY()->step();
				this->currentCoordinateSystem->setCurrentY(this->currentCoordinateSystem->getCurrentY() + dirY * this->yStepLen);
				steppedY++;
			}
			else {
				this->stepperManager->getY()->disable();
				yDone = true;
			}
		}

		if (this->zConfing) { // Z axis code
			this->stepperManager->getZ()->step();
			this->currentCoordinateSystem->setCurrentZ(this->currentCoordinateSystem->getCurrentZ() + dirZ * this->zStepLen);

			if (!this->zInLimit) {
				zPull = false;
			}
			else if (this->zInLimit && !gotZFirstLimit && !zPull) {
				zPull = true;
				gotZFirstLimit = true;
				this->currentCoordinateSystem->setCurrentZ(0.0);
				dirZ *= -1;
				this->stepperManager->getZ()->setDirection(dirZ);
			}
			else if (this->zInLimit && gotZFirstLimit && !zPull) {
				this->zConfing = false;
				dirZ *= -1;
				this->stepperManager->getZ()->setDirection(dirZ);
				zLen = this->currentCoordinateSystem->getCurrentZ() / 2.0;
			}
		}
		else { // moves Z to center
			if (steppedZ < zLen / this->zStepLen) {
				this->stepperManager->getZ()->step();
				this->currentCoordinateSystem->setCurrentZ(this->currentCoordinateSystem->getCurrentZ() + dirZ * this->zStepLen);
				steppedZ++;
			}
			else {
				this->stepperManager->getZ()->disable();
				zDone = true;
			}
		}

		// wait for few microseconds while motors makes their steps
		this->justRunThreads(this->stepperManager->getDelayBetweenSteps() - (micros() - startMicros));
	}

	// put all to zero
	this->currentCoordinateSystem->setCurrentX(0.0);
	this->currentCoordinateSystem->setCurrentY(0.0);
	this->currentCoordinateSystem->setCurrentZ(0.0);
	this->currentCoordinateSystem->makeAllEqual();

	return 0;
}


/*
Just runs thread manager the given time (microseconds).
*/
void Rodent::justRunThreads(long duration) {
	long start = micros();
	long micros_last = micros();
	long micro = 0;
	while (micros() - start <= duration) {
		this->getThreadManager()->runThreads();
		micro += micros() - micros_last;
		micros_last = micros();
		if (micro >= duration) {
			break;
		}
	}
}

/*
Handles an error given (error values are constants in the header file).
*/
void Rodent::handleError(int errorNum) {
	switch (errorNum) {
	case IN_X_LIMIT_ERROR:
		break;
	case IN_Y_LIMIT_ERROR:
		break;
	case IN_Z_LIMIT_ERROR:
		break;
	default: break;
	}
}

/*
Reports light's situation.
*/
void Rodent::reportLights() {
	String lightsOn = this->lights->isOn() ? "1" : "0";
	String r = String(this->lights->getRedValue());
	String g = String(this->lights->getGreenValue());
	String b = String(this->lights->getBlueValue());

	String str = String("R5 L" + lightsOn + " r" + r + " g" + g + " b" + b);
	Serial.println(str);
	Serial1.println(str);
}

/*
Reports sound's situation.
*/
void Rodent::reportSounds() {
	String soundsOn = this->speaker->muted() ? "0" : "1";
	Serial.println("R9 S" + soundsOn);
}

/*
Reports Rodent's mode.
*/
void Rodent::reportMode() {
	String mode = String(this->_mode);
	String str = String("R12 " + mode);
	Serial.println(str);
}

/*
Reports Rodent's lid status. 0 means the lid is closed, 1 means the lid is open.
*/
void Rodent::reportLid() {
	String lid = this->lidOpen ? "1" : "0";
	Serial.println("R7 L" + lid);
}

/*
Reports Rodent's Spindle situation.
*/
void Rodent::reportSpindle() {
	String spindleOn = this->spindle->isSpinning() ? "1" : "0";
	String spindleValue = String(this->spindle->getPWM());
	String str = String("R8 S" + spindleOn + " Q" + spindleValue);
	Serial.println(str);
	Serial1.println(str);
}

/*
Reports an error.
*/
void Rodent::reportError(double diff) {
	Serial.println("R13 Y" + String(diff));
}

/*
Reports Rodent's manual state.
*/
void Rodent::reportManualState() {
	String value = this->inManualMode ? "1" : "0";
	Serial.println("R14 M" + value);
}

/*
Reports Rodent's unit mode.
*/
void Rodent::reportUnitMode() {
	String value = this->millimetricalProgramming ? "1" : "0";
	Serial.println("R17 M" + value);
}

/*
Reports Rodent's relative mode.
*/
void Rodent::reportRelativeMode() {
	String value = this->absoluteProgramming ? "1" : "0";
	Serial.println("R18 M" + value);
}

/*
Reports Rodent's stop mode.
*/
void Rodent::reportStopMode() {
	if (this->compulsoryCancel || this->programCompulsoryCancel) {
		Serial.println("R26 M1");
	}
	else {
		Serial.println("R26 M0");
	}
}

/*
Sets an automatic or manual state based on the boolean value.
True means the Rodent is in manual mode, false says the Rodent is in automatic mode.
*/
void Rodent::setManualState(bool manual) {
	if (manual != this->inManualMode) {
		manual ? this->setRelativeMode(false) : this->setRelativeMode(false);
		this->inManualMode = manual;
		this->speaker->play(new Sound(*NOTIFICATION_SOUND));
		this->reportManualState();
		this->clearEncoder();
		this->realCoordinateSystem->setCurrentX(this->machineCoordinateSystem->getCurrentX());
	}

}

/*
Sets the lid open boolean flag based on the boolean value. If value changes, reports new value
to the controlling software.
*/
void Rodent::setLidState(bool lidOpen) {
	if (lidOpen != this->lidOpen) {
		this->lidOpen = lidOpen;
		this->reportLid();
		if (lidOpen) {
			this->speaker->play(new Sound(*LID_OPENED));
		}
	}
}

/*
Sets the emergency button state and the stop state. If the button is pressed, the code execution will
be stopped immediately.
*/
void Rodent::setEmergencyButtonState(bool pressed) {
	if (pressed && !this->compulsoryCancel) {
		this->stop();
	}
	else if(!pressed){
		bool cancelled = this->compulsoryCancel;
		this->compulsoryCancel = false;
		cancelled != pressed ? this->mayStartAgain() : void();
	}
}


/*
Checks if not stopped either from the emergency button or from the program.
If not stopped, program may start again.
*/
void Rodent::mayStartAgain() {
	!this->programCompulsoryCancel && !this->compulsoryCancel ? Serial.println("R25") : 0;
}

/*
Stops code execution in program.
*/
void Rodent::programStop() {
	this->workCoordinateSystem->makeAllEqual();
	this->machineCoordinateSystem->makeAllEqual();
	this->stepperManager->disableAll();
	this->hasToMove(false);
	this->programCompulsoryCancel = true;
	this->reportStopMode();
	this->moving = false;
}

/*
Enables the program execution again.
*/
void Rodent::programEnable() {
	this->programCompulsoryCancel = false;
	this->mayStartAgain();
}

/*
Sets the unit mode of the Rodent. Give true if you want it to be millimetric, else give false.
*/
void Rodent::setUnitMode(bool millimetric) {
	this->millimetricalProgramming = millimetric;
	this->reportUnitMode();
}

/*
Sets the relative mode of the Rodent. Give true if you want it to be in relative mode.
*/
void Rodent::setRelativeMode(bool relative) {
	this->absoluteProgramming = !relative;
	this->reportRelativeMode();
}

/*
Sets RodentStatus object and gets value from it.
*/
void Rodent::setStatus(StatusObject status) {
	this->lights->setRedValue(status.red);
	this->lights->setGreenValue(status.green);
	this->lights->setBlueValue(status.blue);
	status.lightsOn ? this->lights->on() : this->lights->off();
	this->machineCoordinateSystem->setCurrentX(status.machineX);
	this->machineCoordinateSystem->setCurrentY(status.machineY);
	this->machineCoordinateSystem->setCurrentZ(status.machineZ);
	this->machineCoordinateSystem->makeAllEqual();
}

/*
Plays an alert sound.
*/
void Rodent::playAlertSound() {
	this->speaker->play(new Sound(*NOTIFICATION_SOUND));
}

void Rodent::clearEncoder() {
	Serial2.println("R1");
	Serial3.println("R1");
	this->gotRealX = false;
	this->gotRealY = false;
	this->realCoordinateSystem->setCurrentX(this->machineCoordinateSystem->getCurrentX());
	this->realCoordinateSystem->setCurrentY(this->machineCoordinateSystem->getCurrentY());
}

double Rodent::requestRealCoordinatesX() {
	long start = millis();
	long timeout = 20;
	Serial2.println("R0");
	while (millis() - start <= timeout) {
		this->threadManager->runThreads();
		if (this->gotRealX) {
			break;
		}
	}

	return this->realCoordinateSystem->getCurrentX();
}

double Rodent::requestRealCoordinatesY() {
	long start = millis();
	long timeout = 20;
	Serial3.println("R0");
	while (millis() - start <= timeout) {
		this->threadManager->runThreads();
		if (this->gotRealY) {
			break;
		}
	}

	return this->realCoordinateSystem->getCurrentY();
}

void Rodent::sendSSID(char *ssid) {
	Serial1.print("SSID\"");
	Serial1.print(ssid);
	Serial1.println("\"");
}

void Rodent::sendPassword(char *pass) {
	Serial1.print("PASS\"");
	Serial.println(pass);
	Serial1.print(pass);
	Serial1.println("\"");
}

void Rodent::connectWiFi() {
	Serial1.println("CONNECT");
}