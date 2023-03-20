#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* player_new(Vector2D position);
Entity* player_get();
Vector2D player_get_position();

#endif