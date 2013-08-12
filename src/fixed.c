#include <math.h>

#include "global.h"

/*inline fixed float_to_fixed (float a)
{
	return (fixed)(a * (1 << FRAC));
}

inline float fixed_to_float (fixed a)
{
	return (float) ((a >> FRAC) + ((float)(a & 0x000f) * (1.0/16.0)));
}*/

inline fixed fmul (fixed a, fixed b)
{
//	return (fixed)((int64) a * b >> FRAC);
	return a * b >> FRAC;
}

inline fixed fdiv (fixed a, fixed b)
{
	return (fixed)(((int64) a << FRAC) / b);
}

#define M_PI 3.141593
static fixed fsin_table [12867];
void fsin_gentable (void)
{
	int i;
	for (i = 0; i < 12867; i++)
		fsin_table [i] = tofixed (sinf (M_PI * 2 * ((float)i / 12867.0)));
}

inline fixed fsin (fixed a)
{
	return fsin_table [(a % tofixed (M_PI * 2)) >> 5];
}
