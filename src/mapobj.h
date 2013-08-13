#ifndef MAPOBJ_H__
#define MAPOBJ_H__

struct point_s;
typedef struct mapobj_s
{
	struct point_s p;
	float angle;
	void (*logic)(struct mapobj_s *self);
} mapobj;

#endif // MAPOBJ_H__
