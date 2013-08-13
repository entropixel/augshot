#include <stdio.h>

#include "sdl.h"

#include "global.h"
#include "input.h"
#include "geom.h"
#include "mapobj.h"
#include "player.h"
#include "rndr.h"
#include "level.h"

#if !SDL_VERSION_ATLEAST(2,0,0)
#error "Need SDL 2.0"
#endif

//#define SPEEDTEST

SDL_Window *win;
SDL_Texture *screen;
SDL_Renderer *rndr;

uint8 running = 1;
uint32 curtick = 0;
static uint32 ticktime = 1000 / 60;
uint16 frametimes [48] = { 0 };

int main (int argc, char **argv)
{
	#ifdef _WIN32 // redirect stdout and stderr
	freopen ("stdout.txt", "w", stdout);
	freopen ("stderr.txt", "w", stderr);
	#endif // _WIN32

	// Initialize SDL
	fprintf (stdout, "Initializing SDL (%s)\n", SDL_GetRevision ());
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		fprintf (stderr, "SDL_Init failure (%s)\n", SDL_GetError ());
		return 1;
	}

	if (!(win = SDL_CreateWindow ("7dfps - " GIT_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SWIDTH * 2, SHEIGHT * 2, 0)))
	{
		fprintf (stderr, "SDL_CreateWindow failure (%s)\n", SDL_GetError ());
		return 2;
	}

	if (!(rndr = SDL_CreateRenderer (win, -1, SDL_RENDERER_ACCELERATED)))
	{
		fprintf (stderr, "SDL_CreateRenderer failure (%s)\n", SDL_GetError ());
		return 3;
	}

	if (!(screen = SDL_CreateTexture (rndr, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SWIDTH, SHEIGHT)))
	{
		fprintf (stderr, "SDL_CreateTexture failure (%s)\n", SDL_GetError ());
		return 4;
	}

	input_keys = SDL_GetKeyboardState (NULL);
	rndr_prepare ();
	level_prepare (level);

	int32 nexttick = SDL_GetTicks ();
	SDL_Event ev;

	#ifdef SPEEDTEST
	int32 starttick = nexttick;
	#endif // SPEEDTEST

	while (running)
	{
		static int32 sleeptime, loops = 0;

		// set tick
		curtick ++;
		nexttick += ticktime;

		// get events
		while (SDL_PollEvent (&ev))
		{
			if (ev.type == SDL_QUIT)
				running = 0;
			if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)
				input_keys = SDL_GetKeyboardState (NULL);
		}

		// do logic here
		player.logic (&player);

		// if our current time is ahead of the start of the next tick, we will skip rendering this frame,
		// in an attempt to catch back up. we'll do this up to 15 times before giving up and rendering
		// this frame anyway. The game itself will slow down at that point.
		if (SDL_GetTicks () > nexttick && loops++ < 15)
			continue;

		loops = 0;

		// rendering code
		rndr_dorndr ();

		// present the result on screen
		#ifndef SPEEDTEST
		SDL_UpdateTexture (screen, NULL, pixels, SWIDTH * 4);
		SDL_RenderCopy (rndr, screen, NULL, NULL);
		SDL_RenderPresent (rndr);

		// sleep until next frame
		sleeptime = nexttick - SDL_GetTicks ();
		if (sleeptime > 0)
		{
			if (sleeptime > ticktime)
				sleeptime = ticktime;

			SDL_Delay (sleeptime);
		}

		frametimes [curtick % 48] = ticktime - sleeptime;
		#else
		if (curtick == 4000)
			break;
		#endif // SPEEDTEST
	}

	#ifdef SPEEDTEST
	int32 st_time = SDL_GetTicks () - starttick;
	printf ("4000 frames in %ims (avg %fms/frame)\n", st_time, (float)st_time / 4000.0);
	#endif // SPEEDTEST

	SDL_DestroyRenderer (rndr);
	SDL_DestroyWindow (win);
	SDL_Quit (); 
	return 0;
}
