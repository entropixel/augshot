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
	void (*func) (void);
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
void rndr_texsetframe (texture *tx, int8 frame);
void rndr_prepare (void);
void rndr_dorndr (void);

#endif // RNDR_H__
