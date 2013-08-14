#include <stdio.h>
#include <math.h>

#include "sdl.h"

#include "global.h"
#include "geom.h"
#include "rndr.h"
#include "player.h"
#include "mapobj.h"
#include "input.h"

uint8 input_keydown (const char *key)
{
	return !!input_keys [SDL_GetScancodeFromName (key)];
}

void input_dokeys (void)
{
	int i;

	// for each key defined, see if it's pressed, and whether we need to run its pressed function
	for (i = 0; keydefs [i].name; i++)
	{
		if (input_keydown (keydefs [i].name))
		{
			// run the function, if the last time this was pressed was more than a tick ago
			if (keydefs [i].last < curtick - 1)
			{
				if (SDL_GetModState () == KMOD_LSHIFT && keydefs [i].sfunc)
					keydefs [i].sfunc ();
				else
					keydefs [i].func ();
			}

			keydefs [i].last = curtick;
		}
	}
}

keydef_t keydefs [] = {
	{ NULL, 0, NULL }
};

void input_movemouse (SDL_MouseMotionEvent *mm)
{
//	SDL_WarpMouseInWindow (NULL, SWIDTH / 2, SHEIGHT / 2);
	player_turn (&player, mm->xrel);
}
