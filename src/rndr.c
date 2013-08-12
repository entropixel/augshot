#include <math.h>

#include "global.h"

#include "geom.h"
#include "rndr.h"

uint8 *pixels;
float pplut [SWIDTH] = { 0 }; // LUT for angles pointing to points on the projection plane
float ppdist; // distance to projection plane center, from the player

static inline void rndr_setpixel (uint32 x, uint32 y, uint8 r, uint8 g, uint8 b)
{
	*(uint32*)(pixels + (y * SWIDTH * 4) + (x * 4)) = 0xff000000 | (r << 16) | (g << 8) | b;
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

static inline float distcalc (point a, point b)
{
	return sqrtf ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

#define NUMLINES 4
static line lines [NUMLINES] = {
	{ { -200, 200 }, { 200, 200 } },
	{ { 200, 200 }, { 200, -200 } },
	{ { 200, -200 }, { -200, -200 } },
	{ { -200, -200 }, { -200, 200 } }
};

// render wall section
void rndr_column (uint32 x, line *ray, line *wall, point in, int color)
{
	int i;
	float dist = distcalc (ray->a, in) * cosf (-pplut [x]);
	float colh = (128.0 / dist) * ppdist;
	float offsx, offsy;

	for (i = SHEIGHT / 2 - colh * 0.5; i < SHEIGHT / 2 + colh * 0.5; i++)
	{
		if (i < 0 || i >= SHEIGHT)
			continue;

		offsx = distcalc (wall->a, in);

		if ((int)(offsx / 8) % 2)
			rndr_setpixel (x, i, color, color, color);
		else
			rndr_setpixel (x, i, color + 40, color, color);
	}
}

camera player = { { 0, 0 }, 0.0 };

#define RNDRDIST 1000
void rndr_dorndr (void)
{
	int i, j;
	line ray;
	point in;

	rndr_clear ();

	ray.a.x = player.p.x;
	ray.a.y = player.p.y;

	for (i = 0; i < SWIDTH; i++)
	{
		// cast a ray for every screen pixel
		// end point will fall off in the distance
		ray.b.x = ray.a.x + cosf (player.angle + pplut [i]) * RNDRDIST;
		ray.b.y = ray.a.y + sinf (player.angle + pplut [i]) * RNDRDIST;

		// for each line on the level, test intersection and render it
		// FIXME: This is terrible, it tests against lines we can't see
		for (j = 0; j < NUMLINES; j++)
		{
			if (intersect (&ray, &(lines [j]), &in))
				rndr_column (i, &ray, &(lines [j]), in, 64 + j * 16);
		}
	}

	player.angle += 0.01;
}
