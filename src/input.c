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
	player_turn (&player, mm->xrel);
}

uint8 input_mousedown (int button)
{
	return SDL_GetMouseState (NULL, NULL) & SDL_BUTTON (button);
}
