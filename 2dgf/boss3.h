#ifndef __BOSS3_H__
#define __BOSS3_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* boss3_new(Vector2D position);
Vector2D boss3_get_position(Entity* self);

#endif
