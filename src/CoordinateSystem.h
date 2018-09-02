#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H
#define MM_IN_INCHES 0.0393700787
#define INCHES_IN_MM 25.4

class CoordinateSystem {

private:
	double currentX;
	double currentY;
	double currentZ;
	bool xMirrored;
	bool yMirrored;
	bool zMirrored;
	double targetX;
	double targetY;
	double targetZ;
	bool isMM;

public:
	CoordinateSystem();
	void init();
	void mirrorX(bool);
	void mirrorY(bool);
	void mirrorZ(bool);
	bool isXMirrored();
	bool isYMirrored();
	bool isZMirrored();
	void setZero();
	void setZeroX();
	void setZeroY();
	void setZeroZ();
	double getCurrentX();
	void setCurrentX(double);
	double getCurrentY();
	void setCurrentY(double);
	double getCurrentZ();
	void setCurrentZ(double);
	void offsetX(double);
	void offsetY(double);
	void offsetZ(double);
	void setTargetX(double);
	void setTargetY(double);
	void setTargetZ(double);
	double getTargetX();
	double getTargetY();
	double getTargetZ();
	void makeAllEqual();
	void setToMM();
	void setToInches();
};

#endif