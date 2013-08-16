#include "global.h"
#include "xor.h"

xorshift internal;

inline void xsrand (uint32 seed)
{
	xsrand_r (&internal, seed);
}

void xsrand_r (xorshift *x, uint32 seed)
{
	if (!seed)
		x->rs [0] = seed - 1;
	else
		x->rs [0] = seed;

	x->seed = x->rs [0];

	x->rs [1] = x->rs [0] << 1;
	x->rs [2] = x->rs [1] << 1;
	x->rs [3] = x->rs [2] << 1;
}

inline uint32 xrand (void)
{
	return xrand_r (&internal);
}

uint32 xrand_r (xorshift *x)
{
	register uint32 tmp;

	tmp = x->rs [0] ^ (x->rs [0] << 4);

	x->rs [0] = x->rs [1];
	x->rs [1] = x->rs [2];
	x->rs [2] = x->rs [3];

	return x->rs [3] = (x->rs [3] ^ (x->rs [3] >> 13)) ^ (tmp ^ (tmp >> 13));
}

inline float fxrand (void)
{
	return fxrand_r (&internal);
}

// returns random number in the range 0..1.0
float fxrand_r (xorshift *x)
{
	return (float)(xrand_r (x) % 10000) / 10000.0;
}
