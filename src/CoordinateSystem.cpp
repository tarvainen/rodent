#include "CoordinateSystem.h"

/*
Default constructor.
*/
CoordinateSystem::CoordinateSystem() {
	this->init();
}

/*
Initializes all the values to their defaults.
*/
void CoordinateSystem::init() {
	this->xMirrored = false;
	this->yMirrored = false;
	this->zMirrored = false;
	this->currentX = 0;
	this->currentY = 0;
	this->currentZ = 0;
	this->targetX = 0;
	this->targetY = 0;
	this->targetZ = 0;
	this->isMM = true;
}

/*
Sets the X axis target value.
*/
void CoordinateSystem::setTargetX(double coord) {
	this->targetX = coord;
}

/*
Returns the X axis target value.
*/
double CoordinateSystem::getTargetX() {
	return this->targetX;
}

/*
Returns the current coordinate on X axis.
*/
double CoordinateSystem::getCurrentX() {
	return this->currentX;
}

/*
Sets current X coordinate.
*/
void CoordinateSystem::setCurrentX(double coord) {
	this->currentX = coord;
}

/*
Sets the target coordinate on Y axis.
*/
void CoordinateSystem::setTargetY(double coord) {
	this->targetY = coord;
}

/*
Returns the target coordinate on Y axis.
*/
double CoordinateSystem::getTargetY() {
	return this->targetY;
}

/*
Returns the current coordinate on Y axis.
*/
double CoordinateSystem::getCurrentY() {
	return this->currentY;
}

/*
Sets the current coordinate on X axis.
*/
void CoordinateSystem::setCurrentY(double coord) {
	this->currentY = coord;
}

/*
Sets the target coordinate on Z axis.
*/
void CoordinateSystem::setTargetZ(double coord) {
	this->targetZ = coord;
}

/*
Returns the target coordinate on Z axis.
*/
double CoordinateSystem::getTargetZ() {
	return this->targetZ;
}

/*
Returns the current coordinate on Z axis.
*/
double CoordinateSystem::getCurrentZ() {
	return this->currentZ;
}

/*
Sets the current coordinate on Z axis.
*/
void CoordinateSystem::setCurrentZ(double coord) {
	this->currentZ = coord;
}

/*
Mirrors the X axis. Flips the both, current and target, values to the other side.
*/
void CoordinateSystem::mirrorX(bool mirrored) {
	if ((!this->xMirrored && mirrored) || (this->xMirrored && !mirrored)) {
		this->xMirrored = mirrored;
		this->currentX *= -1;
		this->targetX *= -1;
	}
}

/*
Mirrors the Y axis. Flips the both, current and target, values to the other side.
*/
void CoordinateSystem::mirrorY(bool mirrored) {
	if ((!this->yMirrored && mirrored) || (this->yMirrored && !mirrored)) {
		this->yMirrored = mirrored;
		this->currentY *= -1;
		this->targetY *= -1;
	}
}

/*
Mirrors the Z axis. Flips the both, current and target, values to the other side.
*/
void CoordinateSystem::mirrorZ(bool mirrored) {
	if ((!this->zMirrored && mirrored) || (this->zMirrored && !mirrored)) {
		this->zMirrored = mirrored;
		this->currentZ *= -1;
		this->targetZ *= -1;
	}
}

/*
Makes all values equal (no difference between target and current values).
Is used for canceling movement.
*/
void CoordinateSystem::makeAllEqual() {
	this->targetX = this->currentX;
	this->targetY = this->currentY;
	this->targetZ = this->currentZ;
}

/*
Converts all units to mm if in inches.
*/
void CoordinateSystem::setToMM() {
	if (!this->isMM) {
		this->isMM = true;
		this->currentX *= INCHES_IN_MM;
		this->currentY *= INCHES_IN_MM;
		this->currentZ *= INCHES_IN_MM;
		this->targetX *= INCHES_IN_MM;
		this->targetY *= INCHES_IN_MM;
		this->targetZ *= INCHES_IN_MM;
	}
}

/*
Converts all units to inches if in mm.
*/
void CoordinateSystem::setToInches() {
	if (this->isMM) {
		this->isMM = false;
		this->currentX *= MM_IN_INCHES;
		this->currentY *= MM_IN_INCHES;
		this->currentZ *= MM_IN_INCHES;
		this->targetX *= MM_IN_INCHES;
		this->targetY *= MM_IN_INCHES;
		this->targetZ *= MM_IN_INCHES;
	}
}

/*
Zeros all the coordinates.
*/
void CoordinateSystem::setZero() {
	this->setZeroX();
	this->setZeroY();
	this->setZeroZ();
}

/*
Zeros the X coordinate.
*/
void CoordinateSystem::setZeroX() {
	this->currentX = 0;
	this->targetX = 0;
}

/*
Zeros the Y coordinate.
*/
void CoordinateSystem::setZeroY() {
	this->currentY = 0;
	this->targetY = 0;
}

/*
Zeros the Z coordinate.
*/
void CoordinateSystem::setZeroZ() {
	this->currentZ = 0;
	this->targetZ = 0;
}