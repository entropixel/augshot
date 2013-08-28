/*
	Copyright (c) 2013 Kyle Davis, All Rights Reserved.

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

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
