#ifndef __GREENENEMY_H__
#define __GREENENEMY_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* blueEnemy_new(Vector2D position);
Vector2D greenEnemy_get_position(Entity* self);

#endif

#pragma once
