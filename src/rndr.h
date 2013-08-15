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
	uint8 next;
} animframe;

typedef struct
{
	const char *path;
	uint32 *pixels;
	int32 w;
	int32 h;

	int32 rw;

	animframe *frames;
	uint8 curframe;
	int8 dur;
} texture;

typedef struct mapsprite_s
{
	point *p;
	float dist;
	texture tx;
	struct mapsprite_s *next;
} mapsprite;

void rndr_prepare (void);
void rndr_dorndr (void);

#endif // RNDR_H__
