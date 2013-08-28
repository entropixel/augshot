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

#include <stdio.h>
#include <math.h>

#include "global.h"

#include "geom.h"
#include "mapobj.h"
#include "level.h"
#include "player.h"
#include "rndr.h"

mapobj *objlist = NULL;

void mapobj_add (mapobj *m)
{
	m->next = objlist;
	objlist = m;
}

uint8 mapobj_line_coll (mapobj *m, line *ln, uint8 correct)
{
	float dist_ab = distcalc (ln->a, ln->b), dist_cproj;
	point d, proj;

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
		if (correct)
		{
			m->collisions [m->collidx].dx = ((m->p.x - proj.x) / dist_cproj) * (m->radius - dist_cproj);
			m->collisions [m->collidx].dy = ((m->p.y - proj.y) / dist_cproj) * (m->radius - dist_cproj);
			m->collidx ++;
		}
		return 1;
	}

	return 0;
}

uint8 mapobj_obj_coll (mapobj *m, mapobj *t, uint8 correct)
{
	float dist_mt = distcalc (m->p, t->p) - t->radius;

	if (dist_mt < m->radius)
	{
		if (correct)
		{
			m->collisions [m->collidx].dx = ((m->p.x - t->p.x) / dist_mt) * (m->radius - dist_mt);
			m->collisions [m->collidx].dy = ((m->p.y - t->p.y) / dist_mt) * (m->radius - dist_mt);
			m->collidx ++;
		}
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

// This probably is pretty horrible, but should suffice, as we need a place off of the stack to store bullet info:
point bulletfifo [1024];
uint32 bfidx = 0;

void mapobj_shoot (mapobj *m, float angle)
{
	int lx, ly;
	line hitscan = { { m->p.x, m->p.y } };
	point bestpt, in;
	linelist *it;
	mapobj *mit = objlist, *bestobj = NULL;
	float compdist, bestdist = 8192.0;
	float mcos = cosf (angle), msin = sinf (angle);
	// cast the shot out
	hitscan.b.x = hitscan.a.x + mcos * 8192.0;
	hitscan.b.y = hitscan.a.y + msin * 8192.0;

	lx = (uint32)(player.p.x / 256.0);
	ly = (uint32)(player.p.y / 256.0);
	it = level [lx] [ly].visible;

	// find closest wall that intersects the hitscan
	while (it)
	{
		if (intersect (&hitscan, it->l, &in))
			compdist = distcalc (hitscan.a, in);
		else
		{
			it = it->next;
			continue;
		}

		if (compdist < bestdist)
			bestdist = compdist;

		it = it->next;
	}

	if (bestdist == 8192.0) // no hits
		return;

	// We need to move the hit a little bit away from the wall, so it doesn't clip with the wall
	bestpt.x = hitscan.a.x + mcos * (bestdist - 8.0);
	bestpt.y = hitscan.a.y + msin * (bestdist - 8.0);
	hitscan.b.x = bestpt.x;
	hitscan.b.y = bestpt.y;

	// find closest object that the line intersects, if any
	while (mit)
	{
		if (mit == m)
			goto next;

		if (mapobj_line_coll (mit, &hitscan, 0))
			compdist = distcalc (m->p, mit->p);
		else
			goto next;
	

		if (compdist < bestdist)
			bestobj = mit;

		next:
		mit = mit->next;
	}

	if (bestobj)
	{
		// damage here
	}

	// copy our intersection over to the fifo, so it doesn't go out of scope when we need it later, then create a bullet puff
	bulletfifo [bfidx] = bestpt;
	rndr_addsprite (&plastex, &(bulletfifo [bfidx]), 1);

	bfidx = (++bfidx) % 1024;
}
