#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "global.h"

#include "geom.h"
#include "mapobj.h"

uint8 mapobj_line_coll (mapobj *m, line *ln)
{
	float dist_ab = distcalc (ln->a, ln->b), dist_cproj;
	point d, proj;

	// check for any weirdness
	assert (ln->a.x <= ln->b.x && ln->a.y <= ln->b.y);

	// directional vector from a to b
	d.x = (ln->b.x - ln->a.x) / dist_ab;
	d.y = (ln->b.y - ln->a.y) / dist_ab;

	// find t for the point along ab that is closest to c
	float t = d.x * (m->p.x - ln->a.x) + d.y * (m->p.y - ln->a.y);

	// projection of c on ab
	proj.x = t * d.x + ln->a.x;
	proj.y = t * d.y + ln->a.y;

	if (proj.x < ln->a.x)
		proj.x = ln->a.x + 1;
	if (proj.x > ln->b.x)
		proj.x = ln->b.x - 1;
	if (proj.y < ln->a.y)
		proj.y = ln->a.y + 1;
	if (proj.y > ln->b.y)
		proj.y = ln->b.y - 1;

	dist_cproj = distcalc (m->p, proj);
	
	// test collision
	if (dist_cproj < m->radius)
	{
		// push the distance required to make dist_cproj == m->radius
		m->collisions [m->collidx].dx = ((m->p.x - proj.x) / dist_cproj) * (m->radius - dist_cproj);
		m->collisions [m->collidx].dy = ((m->p.y - proj.y) / dist_cproj) * (m->radius - dist_cproj);
		m->collidx ++;
		return 1;
	}

	return 0;
}

void mapobj_correct_coll (mapobj *m)
{
	float bestx = 0.0, besty = 0.0;
	int i;

	for (i = 0; i < m->collidx; i++)
	{
		if (fabs (m->collisions [i].dx) > fabs (bestx))
			bestx = m->collisions [i].dx;

		if (fabs (m->collisions [i].dy) > fabs (besty))
			besty = m->collisions [i].dy;
	}

	m->collidx = 0;
	m->p.x += bestx;
	m->p.y += besty;
}
