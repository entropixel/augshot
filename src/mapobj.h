/*
	Copyright (c) 2013 Kyle Davis, All Rights Reserved.

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

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
