#ifndef __KEHOEBOSS_H__
#define __KEHOEBOSS_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* kehoe_new(Vector2D position);
Vector2D kehoe_get_position(Entity* self);

#endif
