#ifndef __WEAPONS_H_
#define __WEAPONS_H_

#include "gfc_types.h"
#include "gfc_vector.h"
#include "entity.h"
#include "gf2d_sprite.h"

typedef enum {
	Pistol,
	Laser,
	Shotgun,
	MachineGun,
	Knife
}WeaponType;

typedef struct Weapon_S {
	WeaponType type;
	Entity* owner;
	Sprite* sprite;
	Vector2D aimPos;
	float damage;
	float range;
	float firerate;
	float projectileVelocity;
}WeaponData;

/**
* @brief initialize ther internal manager for the weapon entity
*
*/

Entity* new_weapon(WeaponType type);
void weapon_free(Entity* ent);
void weapon_update(Entity* self);
void weapon_think(Entity* self);

#endif