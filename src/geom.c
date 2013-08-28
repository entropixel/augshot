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
