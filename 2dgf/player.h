#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* player_new(Vector2D position);
Entity* player_get();
Vector2D player_get_position(Entity* self);
Uint8 entity_clip_with_player(Entity* ent);
int player_getNumOfBullets();
void player_setNumOfBullets(int num);
int player_getHealth();
void player_setHealth(int num);

#endif