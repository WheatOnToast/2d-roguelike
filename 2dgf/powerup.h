#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* powerup_new(Vector2D position);
int powerup_get_powerUpCount();
void powerup_set_powerUpCount(int num);


#endif
