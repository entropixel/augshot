#include <math.h>

#include "global.h"

#include "input.h"
#include "geom.h"
#include "mapobj.h"
#include "player.h"

mapobj player = { { 64, 64 }, 0.0, player_logic };

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
}
