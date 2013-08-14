#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "global.h"

#include "geom.h"
#include "rndr.h"
#include "mapobj.h"
#include "player.h"
#include "level.h"

#define LIGHTGRAD 0.015
uint32 *pixels;
float pplut [SWIDTH] = { 0 }; // LUT for angles pointing to points on the projection plane
float ppdist; // distance to projection plane center, from the player

static inline void rndr_setpixel (uint32 x, uint32 y, uint8 r, uint8 g, uint8 b)
{
	*(pixels + (y * SWIDTH) + x) = 0xff000000 | (r << 16) | (g << 8) | b;
}

static inline void rndr_darken (uint32 x, uint32 y, float r, float g, float b, float mul)
{
	rndr_setpixel (x, y, (uint8)(r * mul), (uint8)(g * mul), (uint8)(b * mul));
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
// returns bottom pixel
uint32 rndr_column (float dist, uint32 x, line *ray, line *wall, point in, int color)
{
	int i;
	float offsx, offsy;
	float colh = (128.0 / dist) * ppdist;
	float dark = 1 / (dist * LIGHTGRAD);
	dark = dark > 1 ? 1 : dark;

	offsx = distcalc (wall->a, in);

	for (i = SHEIGHT / 2 - colh * 0.5; i < SHEIGHT / 2 + colh * 0.5; i++)
	{
		if (i < 0 || i >= SHEIGHT)
			continue;

		if ((int)(offsx / 8) % 2)
			rndr_darken (x, i, color, color, color, dark);
		else
			rndr_darken (x, i, color + 40, color, color, dark);
	}

	return i;
}

// render floor
void rndr_floor (uint32 x, uint32 start)
{
	// TODO: maybe save straight distances in a LUT, speeding up the calculation
	int i;
	float sdist, adist;
	float dark;

	// precalc these
	float pplutcos = cosf (-pplut [x]);
	float plcos = cosf (player.angle + pplut [x]);
	float plsin = sinf (player.angle + pplut [x]);

	// coords to the pixel
	int32 pxx, pxy;

	for (i = start; i < SHEIGHT; i++)
	{
		sdist = ppdist * (64.0 / (i - SHEIGHT / 2));
		adist = sdist / pplutcos;
		pxx = player.p.x + plcos * adist;
		pxy = player.p.y + plsin * adist;

		dark = 1 / (sdist * LIGHTGRAD);
		dark = dark > 1 ? 1 : dark;
		rndr_darken (x, i, 32, 32, 32, dark);
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
	uint32 lx, ly, colend;
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
		if (best)
		{
			colend = rndr_column (bestdist, i, &ray, best, bestpt, 48);
			rndr_floor (i, colend);
		}
		else
			rndr_floor (i, SHEIGHT / 2);
	}

	// draw debug stuff (render times)
	for (i = 0; i < 48; i++)
		rndr_setpixel (i, SHEIGHT - 1 - frametimes [i], 200, 200, 0);

//	player.angle += 0.01;
}
