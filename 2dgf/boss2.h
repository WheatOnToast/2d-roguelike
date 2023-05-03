#ifndef __BOSS2_H__
#define __BOSS2_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* boss2_new(Vector2D position);
Vector2D boss2_get_position(Entity* self);

#endif
