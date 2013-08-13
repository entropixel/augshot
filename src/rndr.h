#ifndef RNDR_H__
#define RNDR_H__

extern uint32 *pixels;

#define FOV 3.141593 / 2.0

typedef struct
{
	point p;
	float angle;
} camera;

void rndr_prepare (void);
void rndr_dorndr (void);

#endif // RNDR_H__
