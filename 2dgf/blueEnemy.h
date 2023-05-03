#ifndef __BLUEENEMY_H__
#define __BLUEENEMY_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* blueEnemy_new(Vector2D position);
Vector2D blueEnemy_get_position(Entity* self);

#endif

