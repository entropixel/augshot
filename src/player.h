#ifndef PLAYER_H__
#define PLAYER_H__

struct mapobj_s;
extern struct mapobj_s player;

void player_logic (struct mapobj_s *self);

#endif // PLAYER_H__
