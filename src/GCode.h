#ifndef GCODE_H
#define GCODE_H

class GCode {
private:
	double g;
	double x;
	double y;
	double z;
	double i;
	double j;
	double m;
	double s;
	double f;
	double r;
	double p;
	double q;

	double R;
	double G;
	double B;

	char *str;

	bool g_got;
	bool x_got;
	bool y_got;
	bool z_got;
	bool i_got;
	bool j_got;
	bool m_got;
	bool s_got;
	bool f_got;
	bool r_got;
	bool p_got;
	bool q_got;
	bool R_got;
	bool G_got;
	bool B_got;
	bool str_got;

public:
	GCode();
	~GCode() { delete this->str; this->str = 0;}
	double getG() { return this->g; }
	double getX() { return this->x; }
	double getY() { return this->y; }
	double getZ() { return this->z; }
	double getI() { return this->i; }
	double getJ() { return this->j; }
	double getM() { return this->m; }
	double getS() { return this->s; }
	double getF() { return this->f; }
	double getR() { return this->r; }
	double getP() { return this->p; }
	double getQ() { return this->q; }
	double get_r() { return this->R; }
	double get_g() { return this->G; }
	double get_b() { return this->B; }
	char *getString() { return this->str; }



	double setG(double code) { this->g = code; this->g_got = true; }
	double setX(double code) { this->x = code; this->x_got = true; }
	double setY(double code) { this->y = code; this->y_got = true; }
	double setZ(double code) { this->z = code; this->z_got = true; }
	double setI(double code) { this->i = code; this->i_got = true; }
	double setJ(double code) { this->j = code; this->j_got = true; }
	double setM(double code) { this->m = code; this->m_got = true; }
	double setS(double code) { this->s = code; this->s_got = true; }
	double setF(double code) { this->f = code; this->f_got = true; }
	double setR(double code) { this->r = code; this->r_got = true; }
	double setP(double code) { this->p = code; this->p_got = true; }
	double setQ(double code) { this->q = code; this->q_got = true; }
	double set_r(double code) { this->R = code; this->R_got = true; }
	double set_g(double code) { this->G = code; this->G_got = true; }
	double set_b(double code) { this->B = code; this->B_got = true; }
	void setString(char *str) { this->str = str; this->str_got = true; }


	bool isG() { return this->g_got; }
	bool isX() { return this->x_got; }
	bool isY() { return this->y_got; }
	bool isZ() { return this->z_got; }
	bool isJ() { return this->j_got; }
	bool isI() { return this->i_got; }
	bool isM() { return this->m_got; }
	bool isF() { return this->f_got; }
	bool isS() { return this->s_got; }
	bool isR() { return this->r_got; }
	bool isP() { return this->p_got; }
	bool isQ() { return this->q_got; }
	bool is_r() { return this->R_got; }
	bool is_g() { return this->G_got; }
	bool is_b() { return this->B_got; }
	bool is_str() { return this->str_got; }

};

#endif