#ifndef MAPOBJ_H__
#define MAPOBJ_H__

typedef struct
{
	float dx;
	float dy;
} collision;

struct point_s;
typedef struct mapobj_s
{
	struct point_s p;
	float angle;
	float radius;
	void (*logic)(struct mapobj_s *self);

	float momx;
	float momy;
	uint8 collidx;
	collision collisions [256];
} mapobj;

struct line_s;
uint8 mapobj_line_coll (mapobj *m, struct line_s *ln);
void mapobj_correct_coll (mapobj *m);

#endif // MAPOBJ_H__
