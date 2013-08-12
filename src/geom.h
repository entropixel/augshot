#ifndef GEOM_H__
#define GEOM_H__

typedef struct
{
	float x;
	float y;
} point;

typedef struct
{
	point a;
	point b;
} line;

uint32 intersect (line *a, line *b, point *i);

#endif // GEOM_H__
