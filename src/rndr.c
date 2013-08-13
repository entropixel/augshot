#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "global.h"

#include "input.h"
#include "geom.h"
#include "rndr.h"
#include "mapobj.h"
#include "player.h"
#include "level.h"

uint32 *pixels;
float pplut [SWIDTH] = { 0 }; // LUT for angles pointing to points on the projection plane
float ppdist; // distance to projection plane center, from the player

static inline void rndr_setpixel (uint32 x, uint32 y, uint8 r, uint8 g, uint8 b)
{
	*(pixels + (y * SWIDTH) + x) = 0xff000000 | (r << 16) | (g << 8) | b;
}

static inline void rndr_clear (void)
{
	memset (pixels, 0, SWIDTH * SHEIGHT * 4);
}

void rndr_prepare (void)
{
	int i;

	pixels = malloc (4 * SWIDTH * SHEIGHT);

	ppdist = ((float)SWIDTH / 2.0) / tanf (FOV / 2.0);

	for (i = 0; i < SWIDTH; i++)
		pplut [i] = atanf ((((float)SWIDTH / 2.0 - i)) / ppdist);
}

// render wall section
void rndr_column (float dist, uint32 x, line *ray, line *wall, point in, int color)
{
	int i;
	float colh = (128.0 / dist) * ppdist;
	float offsx, offsy;

	offsx = distcalc (wall->a, in);

	for (i = SHEIGHT / 2 - colh * 0.5; i < SHEIGHT / 2 + colh * 0.5; i++)
	{
		if (i < 0 || i >= SHEIGHT)
			continue;

		if ((int)(offsx / 8) % 2)
			rndr_setpixel (x, i, color, color, color);
		else
			rndr_setpixel (x, i, color + 40, color, color);
	}
}

static inline float rndr_col_distcalc (uint32 x, line *ray, point in)
{
	return distcalc (ray->a, in) * cosf (-pplut [x]);
}

#define RNDRDIST 8192
extern uint16 frametimes [48];
void rndr_dorndr (void)
{
	int i;
	uint32 lx, ly;
	line ray;
	point in;
	tile *t;
	linelist *it;

	line *best;
	point bestpt;
	float bestdist, compdist;

	rndr_clear ();

	ray.a.x = player.p.x;
	ray.a.y = player.p.y;

	for (i = 0; i < SWIDTH; i++)
	{
		bestdist = RNDRDIST;
		best = NULL;

		// cast a ray for every screen pixel
		// end point will fall off in the distance
		ray.b.x = ray.a.x + cosf (player.angle + pplut [i]) * RNDRDIST;
		ray.b.y = ray.a.y + sinf (player.angle + pplut [i]) * RNDRDIST;

		// render each line visible from where the player is
		lx = (uint32)(player.p.x / 256.0);
		ly = (uint32)(player.p.y / 256.0);
		it = level [lx] [ly].visible;
		while (it)
		{
			if (intersect (&ray, it->l, &in))
				compdist = rndr_col_distcalc (i, &ray, in);
			else
			{
				it = it->next;
				continue;
			}

			if (compdist < bestdist)
			{
				bestdist = compdist;
				best = it->l;
				bestpt = in;
			}

			it = it->next;
		}

		// render best choice
		rndr_column (bestdist, i, &ray, best, bestpt, 48);
	}

	// draw debug stuff (render times)
	for (i = 0; i < 48; i++)
		rndr_setpixel (i, SHEIGHT - 1 - frametimes [i], 200, 200, 0);

//	player.angle += 0.01;
}
