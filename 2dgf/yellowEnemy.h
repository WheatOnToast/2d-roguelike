#ifndef __YELLOWENEMY_H__
#define __YELLOWENEMY_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* yellowEnemy_new(Vector2D position);
Vector2D yellowEnemy_get_position(Entity* self);

#endif

#pragma once
