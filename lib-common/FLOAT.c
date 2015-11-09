#include "FLOAT.h"
#include "inttypes.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	int overflow = 0;
	int64_t result = a * b / (1<<16);
	if(result & 0xffffffff00000000) overflow = 1;
	nemu_assert(!overflow);
	return (int)result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	int result = a * (1<<16) /b;
	return result;
}

FLOAT f2F(float a) {
	//nemu_assert(0);
	return 0;
}

FLOAT Fabs(FLOAT a) {
	int result = a & 0x80000000;
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

