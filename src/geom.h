#ifndef GEOM_H__
#define GEOM_H__

typedef struct point_s
{
	float x;
	float y;
} point;

typedef struct line_s
{
	point a;
	point b;
} line;

float distcalc (point a, point b);
uint32 intersect (line *a, line *b, point *i);

#endif // GEOM_H__
