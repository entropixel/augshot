#include <math.h>

#include "global.h"

#include "geom.h"

inline float distcalc (point a, point b)
{
	return sqrtf ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	//return fabs (a.x - b.x) / cosf (angle);
}

uint32 intersect (line *a, line *b, point *i)
{
	point sa = { a->b.x - a->a.x, a->b.y - a->a.y }, sb = { b->b.x - b->a.x, b->b.y - b->a.y };

	float s = (-sa.y * (a->a.x - b->a.x) + sa.x * (a->a.y - b->a.y)) / (-sb.x * sa.y + sa.x * sb.y);
	float t = ( sb.x * (a->a.y - b->a.y) - sb.y * (a->a.x - b->a.x)) / (-sb.x * sa.y + sa.x * sb.y);

	// test intersection
	if (s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0)
	{
		if (i)
		{
			i->x = a->a.x + t * sa.x;
			i->y = a->a.y + t * sa.y;
		}
		return 1;
	}

	return 0;
}
