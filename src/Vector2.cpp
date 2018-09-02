#include "Vector2.h"
#include "Arduino.h"

Vector2::Vector2() {
	this->init();
}

Vector2::Vector2(double x, double y, double size, double dir) {
	this->init();
	this->pos_x = x;
	this->pos_y = y;
	this->size_val = size;
	this->dir = dir;
}

Vector2::Vector2(double startX, double startY, double endX, double endY, bool pointToPoint = true) {
	this->init();
	this->pos_x = startX;
	this->pos_y = startY;
	this->dir = this->angleBetween(startX, startY, endX, endY);
	this->size_val = this->distanceBetween(startX, startY, endX, endY);
}

void Vector2::init() {
	this->pos_x = 0.0;
	this->pos_y = 0.0;
	this->size_val = 0.0;
	this->dir = 0.0;
}

void Vector2::x(double x) {
	this->pos_x = x;
}

void Vector2::y(double y) {
	this->pos_y = y;
}

void Vector2::size(double size) {
	this->size_val = size;
}

void Vector2::direction(double direction) {
	if (direction >= 0 && direction <= 2 * PI) {
		this->dir = direction;
	}
	else if (direction < 0) {
		this->dir = 2 * PI + direction;
	}
	else {
		this->dir = 0 - direction;
	}
}

double Vector2::x() {
	return this->pos_x;
}

double Vector2::y() {
	return this->pos_y;
}

double Vector2::size() {
	return this->size_val;
}

double Vector2::direction() {
	return this->dir;
}

double Vector2::angleBetween(double startX, double startY, double endX, double endY) {
	double dX = endX - startX;
	double dY = endY - startY;

	double angle = 0;

	if (dX == 0 && dY == 0) {
		return -1; // error
	}

	if (dX == 0) {
		if (dY >= 0) {
			return PI / 2;
		}
		else {
			return 6 * PI / 4;
		}
	}

	if (dY == 0) {
		if (dX >= 0) {
			return 0;
		}
		else {
			return PI;
		}
	}

	angle = atan(dY / dX);

	if (dX > 0 && dY > 0) {

	}
	else if (dX < 0 && dY > 0) {
		angle += PI;
	}
	else if (dX < 0 && dY < 0) {
		angle += PI;
	}
	else if (dX > 0 && dY < 0) {
		angle += 2 * PI;
	}

	return angle;
}

double Vector2::distanceBetween(double startX, double startY, double endX, double endY) {
	double dX = abs(endX - startX);
	double dY = abs(endY - startY);
	return sqrt(pow(dX, 2) + pow(dY, 2));
}

double Vector2::endX() {
	if (this->dir == 0) {
		return this->pos_x + this->size_val;
	}
	else if (this->dir == PI){
		return this->pos_x - this->size_val;
	}
	else if (this->dir == PI / 2 || this->dir == (6 * PI) / 4) {
		return 0;
	}
	else if (this->dir > 0 && this->dir < PI / 2) {
		return this->pos_x + cos(this->dir) * this->size_val;
	}
	else if (this->dir > PI / 2 && this->dir < PI) {
		return this->pos_x - cos(PI - this->dir) * this->size_val;
	}
	else if (this->dir > PI && this->dir < (6 * PI) / 4) {
		return this->pos_x - sin((6 * PI) / 4 - this->dir) * this->size_val;
	}
	else {
		return this->pos_x + cos(2 * PI - this->dir) * this->size_val;
	}
}

double Vector2::endY() {
	if (this->dir == 0 || this->dir == PI) {
		return 0;
	}
	else if (this->dir == PI / 2) {
		return this->pos_y + this->size_val;
	}
	else if (this->dir == (6 * PI) / 4) {
		return this->pos_y - this->size_val;
	}
	else if (this->dir > 0 && this->dir < PI / 2) {
		return this->pos_y + sin(this->dir) * this->size_val;
	}
	else if (this->dir > PI / 2 && this->dir < PI) {
		return this->pos_y + sin(PI - this->dir) * this->size_val;
	}
	else if (this->dir > PI && this->dir < (6 * PI) / 4) {
		return this->pos_y - cos((6 * PI) / 4 - this->dir) * this->size_val;
	}
	else {
		return this->pos_y - sin(2 * PI - this->dir) * this->size_val;
	}
}