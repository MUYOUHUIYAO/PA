#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	int q = (1<<16);
	int qa = a / q;
	int qb = b /q;
	int ya = a %q;
	int yb = b % q;
	int result = qa*qb *q + qa * yb +qb*ya + ya * yb/q;
	return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	nemu_assert(b);
	int result = a /b * (1<<16);
	return result + a%b;
}

FLOAT f2F(float a) {
	int args = *(int *)(&a);
	int sign = args & 0x80000000;
	int e = ((args & 0x7f800000)>>23) - 127;
	int m= (args & 0x007fffff) + 0x00800000;
	int result;
	if(e > 7)  result =(m << (e -7));
	else result = (m >> (7 -e));
	if(sign) result = -result;
	return result;
}

FLOAT Fabs(FLOAT a) {
	int result = a & 0x7fffffff;
	return result;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

