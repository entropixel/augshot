#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"

#include "geom.h"
#include "level.h"

/*uint8 leveltmp [LH] [LW] = {
	{ 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 0, 1, 0 },
	{ 0, 0, 1, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0 },
	{ 0, 1, 0, 0, 1, 0 },
	{ 0, 0, 0, 1, 1, 0 },
};*/

uint8 leveltmp [LH] [LW] = {
	{ 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1 },
	{ 0, 0, 1, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0 },
	{ 0, 1, 0, 0, 1, 0 },
	{ 0, 0, 0, 1, 1, 0 },
};

tile level [LW] [LH];

#define GRID 256

// check if there's a clear line of sight to end
static inline uint8 samepoint (point *end, point *test)
{
//	return end->x == test->x && end->y == test->y;
	return fabs (end->x - test->x) <= 1.0 && fabs (end->y - test->y) <= 1.0;
}

static inline point pthack (line *ln, float percent)
{
	point ret;
	if (!ln)
	{
		ret.x = ret.y = 0.0;
		return ret;
	}

	ret.x = ln->a.x + (ln->b.x - ln->a.x) * percent;
	ret.y = ln->a.y + (ln->b.y - ln->a.y) * percent;
//	printf ("point of %f, %f and %f, %f is %f, %f\n", ln->a.x, ln->a.y, ln->b.x, ln->b.y, ret.x, ret.y);
	return ret;
}

static inline uint8 los (line *ray, point end)
{
	int i, j;
	tile *t;

	ray->b.x = end.x;
	ray->b.y = end.y;

	for (i = 0; i < LW; i++)
	for (j = 0; j < LH; j++)
	{
		t = &(level [i] [j]);

		if (t->n && !samepoint (&end, &(t->n->a)) && !samepoint (&end, &(t->n->b)) && intersect (ray, t->n, NULL))
			return 0;

		if (t->e && !samepoint (&end, &(t->e->a)) && !samepoint (&end, &(t->e->b)) && intersect (ray, t->e, NULL))
			return 0;

		if (t->s && !samepoint (&end, &(t->s->a)) && !samepoint (&end, &(t->s->b)) && intersect (ray, t->s, NULL))
			return 0;

		if (t->w && !samepoint (&end, &(t->w->a)) && !samepoint (&end, &(t->w->b)) && intersect (ray, t->w, NULL))
			return 0;

	}

//	printf ("line of sight between %f, %f and %f, %f\n", ray->a.x, ray->a.y, ray->b.x, ray->b.y);
	return 1;
}

static inline void add_visible_line (line *ln, tile *t)
{
	linelist *lst;
	if (!t->visible)
		lst = t->visible = malloc (sizeof (linelist));
	else
	{
		if (ln == t->visible->l)
			return;

		lst = t->visible;
		while (lst->next)
		{
			if (ln == lst->next->l)
				return;

			lst = lst->next;
		}
		lst->next = malloc (sizeof (linelist));
		lst = lst->next;
	}

	lst->l = ln;
	lst->next = NULL;
	t->end_visible = lst;
}

void level_prepare (tile *l)
{
	int i, j;

	// generate lines:
	for (i = 0; i < LW; i++)
	for (j = 0; j < LH; j++)
	{
		tile *t = &(level [i] [j]);

		t->n = NULL;
		t->e = NULL;
		t->s = NULL;
		t->w = NULL;
		t->visible = t->end_visible = NULL;

		if (leveltmp [j] [i] == 0)
			continue;

		line *ln;
		// north wall? if y == 0 or x, y-1 is solid, yes
		if (j == 0 || leveltmp [j - 1] [i] == 0)
		{
			ln = malloc (sizeof (line));
			ln->a.x = i * GRID;
			ln->a.y = j * GRID;
			ln->b.x = (i + 1) * GRID;
			ln->b.y = j * GRID;
			t->n = ln;
		}

		// east wall?
		if (i == LW - 1 || leveltmp [j] [i + 1] == 0)
		{
			ln = malloc (sizeof (line));
			ln->a.x = (i + 1) * GRID;
			ln->a.y = j * GRID;
			ln->b.x = (i + 1) * GRID;
			ln->b.y = (j + 1) * GRID;
			t->e = ln;
		}

		// south wall?
		if (j == LH - 1 || leveltmp [j + 1] [i] == 0)
		{
			ln = malloc (sizeof (line));
			ln->a.x = i * GRID;
			ln->a.y = (j + 1) * GRID;
			ln->b.x = (i + 1) * GRID;
			ln->b.y = (j + 1) * GRID;
			t->s = ln;
		}

		// west wall?
		if (i == 0 || leveltmp [j] [i - 1] == 0)
		{
			ln = malloc (sizeof (line));
			ln->a.x = i * GRID;
			ln->a.y = j * GRID;
			ln->b.x = i * GRID;
			ln->b.y = (j + 1) * GRID;
			t->w = ln;
		}
	}

	// for each tile, make a list of lines that are visible from it
	// this may be inaccurate, but should be "good enough" for now
	for (i = 0; i < LW; i++)
	for (j = 0; j < LH; j++)
	{
		line ray;
		int g, h, o;

		// we start the ray at 4 points in the tile
		static point offs [4] = {
			{ GRID * 0.125, GRID * 0.125 },
			{ GRID * 0.875, GRID * 0.125 },
			{ GRID * 0.125, GRID * 0.875 },
			{ GRID * 0.875, GRID * 0.875 }
		};

		if (leveltmp [j] [i] == 0)
			continue;

		for (o = 0; o < 4; o++)
		{
			ray.a.x = i * GRID + offs [o].x;
			ray.a.y = j * GRID + offs [o].y;
	
			for (g = 0; g < LW; g++)
			for (h = 0; h < LH; h++)
			{
				// don't point *exactly* at the end points, because that will let us see around corners
				point na = pthack (level [g] [h].n, 0.001), nb = pthack (level [g] [h].n, 0.999);
				point ea = pthack (level [g] [h].e, 0.001), eb = pthack (level [g] [h].e, 0.999);
				point sa = pthack (level [g] [h].s, 0.001), sb = pthack (level [g] [h].s, 0.999);
				point wa = pthack (level [g] [h].w, 0.001), wb = pthack (level [g] [h].w, 0.999);
				
				// end ray at the end points and midpoint of each wall of this tile
				if (level [g] [h].n && (los (&ray, na) || los (&ray, nb)))
					add_visible_line (level [g] [h].n, &(level [i] [j]));
	
				if (level [g] [h].e && (los (&ray, ea) || los (&ray, eb)))
					add_visible_line (level [g] [h].e, &(level [i] [j]));
	
				if (level [g] [h].s && (los (&ray, sa) || los (&ray, sb)))
					add_visible_line (level [g] [h].s, &(level [i] [j]));
	
				if (level [g] [h].w && (los (&ray, wa) || los (&ray, wb)))
					add_visible_line (level [g] [h].w, &(level [i] [j]));
			}
		}
	}
}
