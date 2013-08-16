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

	struct mapobj_s *next;
} mapobj;

extern mapobj *objlist;

struct line_s;
void mapobj_add (mapobj *m);
uint8 mapobj_line_coll (mapobj *m, struct line_s *ln, uint8 correct);
uint8 mapobj_obj_coll (mapobj *m, mapobj *t, uint8 correct);
void mapobj_correct_coll (mapobj *m);
void mapobj_shoot (mapobj *m, float angle);

#endif // MAPOBJ_H__
