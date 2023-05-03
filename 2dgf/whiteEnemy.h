#ifndef __REDENEMY_H__
#define __REDENEMY_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* redEnemy_new(Vector2D position);
Vector2D redEnemy_get_position(Entity* self);

#endif
