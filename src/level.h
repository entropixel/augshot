#ifndef LEVEL_H__
#define LEVEL_H__

struct line_s;
struct tile_s;

typedef struct linelist_s
{
	struct line_s *l;
	struct linelist_s *next;
} linelist;

typedef struct tile_s
{
	struct line_s *n;
	struct line_s *e;
	struct line_s *s;
	struct line_s *w;
	linelist *visible;
	linelist *end_visible;
} tile;

#define LW 10
#define LH 10
extern tile level [LW] [LH];

void level_prepare (tile *l);

#endif // LEVEL_H__
