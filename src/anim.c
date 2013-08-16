#include "global.h"
#include "geom.h"
#include "rndr.h"

animframe plasma_frames [] = {
	{ 0, 0, 8, 1 },
	{ 1, 0, 8, 2 },
	{ 0, 1, 8, 3 },
	{ 1, 1, 8, -1 }
};

void player_checkshoot (void);
void player_shoot (void);
animframe gun_frames [] = {
	{ 0, 0, -1, -1 },
	{ 1, 0, 8, 2 },
	{ 2, 0, 8, 3, player_shoot },
	{ 0, 1, 8, 4 },
	{ 1, 1, 8, 5 },
	{ 2, 1, 8, 0 }
};
