#ifndef RNDR_H__
#define RNDR_H__

extern uint32 *pixels;

#define FOV 3.141593 / 2.0

typedef struct
{
	point p;
	float angle;
} camera;

typedef struct
{
	uint8 x;
	uint8 y;
	int8 dur;
	int8 next;
} animframe;

typedef struct
{
	const char *path;
	uint32 *pixels;
	int32 w;
	int32 h;

	int32 rw;

	animframe *frames;
	int8 curframe;
	int8 dur;
} texture;

typedef struct mapsprite_s
{
	point *p;
	float dist;
	texture tx;
	uint8 wall;
	uint8 clear;
	struct mapsprite_s *next;
} mapsprite;

extern texture plastex;

void rndr_addsprite (texture *tx, point *p, uint8 wall);
void rndr_prepare (void);
void rndr_dorndr (void);

#endif // RNDR_H__
