#include <math.h>

#include "global.h"

#include "input.h"
#include "geom.h"
#include "mapobj.h"
#include "level.h"
#include "player.h"

mapobj player = { { 64, 64 }, 0.0, 32.0, player_logic };

void player_logic (mapobj *self)
{
	if (input_keydown ("w"))
	{
		self->p.x += cosf (self->angle) * 3;
		self->p.y += sinf (self->angle) * 3;
	}

	if (input_keydown ("s"))
	{
		self->p.x -= cosf (self->angle) * 3;
		self->p.y -= sinf (self->angle) * 3;
	}

	if (input_keydown ("a"))
	{
		self->p.x += cosf (self->angle + M_PI / 2) * 3;
		self->p.y += sinf (self->angle + M_PI / 2) * 3;
	}

	if (input_keydown ("d"))
	{
		self->p.x += cosf (self->angle - M_PI / 2) * 3;
		self->p.y += sinf (self->angle - M_PI / 2) * 3;
	}

	if (input_keydown ("left"))
	{
		self->angle += 0.03;
	}

	if (input_keydown ("right"))
	{
		self->angle -= 0.03;
	}

	// collision detection
	static int8 offs [5][2] = {
		{ 0, 0 }, { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
	};
	int32 lx, ly, i;
	tile *t;
	lx = (int32)(self->p.x / 256.0);
	ly = (int32)(self->p.y / 256.0);
	t = &(level [lx] [ly]);
	linelist *it = t->visible;

	while (it)
	{
		if (it->l)
			mapobj_line_coll (self, it->l);

		it = it->next;
	}

	#if 0
	// test against each wall of each tile
	for (i = 0; i < 5; i++)
	{
		if (lx + offs [i] [0] < 0 || ly + offs [i] [1] < 0 || lx + offs [i] [0] > LW - 1 || ly + offs [i] [1] > LH - 1)
			continue;

		t = &(level [lx + offs [i] [0]] [ly + offs [i] [1]]);
//		printf ("%i, %i\n", lx + offs [i] [0], ly + offs [i] [1]);

		if (t->n)
			mapobj_line_coll (self, t->n);
		if (t->e)
			mapobj_line_coll (self, t->e);
		if (t->s)
			mapobj_line_coll (self, t->s);
		if (t->w)
			mapobj_line_coll (self, t->w);
	}
	#endif
	mapobj_correct_coll (self);
}

#define MSENS 0.004
void player_turn (mapobj *self, int32 amt)
{
	self->angle -= amt * MSENS;
}
