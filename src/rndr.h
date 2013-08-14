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
	const char *path;
	uint32 *pixels;
	uint16 w;
	uint16 h;
} texture;

void rndr_prepare (void);
void rndr_dorndr (void);

#endif // RNDR_H__
