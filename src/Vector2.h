#ifndef VECTOR_H
#define VECTOR_H

class Vector2{
private:
	double pos_x;
	double pos_y;
	double size_val;
	double dir;
	void init();
public:
	Vector2();
	Vector2(double, double, double, double);
	Vector2(double, double, double, double, bool);
	void x(double);
	void y(double);
	void size(double);
	void direction(double);
	double x();
	double y();
	double size();
	double direction();
	double angleBetween(double, double, double, double);
	double distanceBetween(double, double, double, double);
	double endX();
	double endY();
};


#endif