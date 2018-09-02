#include "GCode.h"

/*
Default constructor. Initializes all the variables to their default values.
*/
GCode::GCode() {
	this->g_got = false;
	this->x_got = false;
	this->y_got = false;
	this->z_got = false;
	this->i_got = false;
	this->j_got = false;
	this->m_got = false;
	this->s_got = false;
	this->f_got = false;
	this->r_got = false;
	this->p_got = false;
	this->q_got = false;
	this->str_got = false;

	this->g = 0.0;
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->i = 0.0;
	this->j = 0.0;
	this->m = 0.0;
	this->s = 0.0;
	this->f = 0.0;
	this->r = 0.0;
	this->p = 0.0;
	this->q = 0.0;
	this->str = 0;
}