#include "simple_logger.h"
#include "gf2d_draw.h"
#include "weapon.h"

Entity* new_weapon(WeaponType type) {
	Entity* weapon = entity_new();
	if (!weapon) {
		return NULL;
	}
	WeaponData* data = gfc_allocate_array(sizeof(WeaponData), 1);
	if (!data) {
		memset(weapon, 0, sizeof(Entity));
		return NULL;
	}
	data->type = type;
	switch (type) {
	case Pistol:
		weapon->sprite = gf2d_sprite_load_all("images/pistol-sprite.png",
			32,
			32,
			1,
			0);
		data->damage = 4;
		data->range = -1;
		data->firerate = 0.6;
		data->projectileVelocity = 1;
		break;
	case Laser:
		weapon->sprite = gf2d_sprite_load_all("images/assaultrifle-sprite.png",
			32,
			32,
			1,
			0);
		data->damage = 3;
		data->range = -1;
		data->firerate = 0.2;
		data->projectileVelocity = 1;
		break;
	case Shotgun:
		weapon->sprite = gf2d_sprite_load_all("images/shotgun-sprite.png",
			32,
			32,
			1,
			0);
		data->damage = 6;
		data->range = -1;
		data->firerate = 1;
		data->projectileVelocity = 1;
		break;
	case MachineGun:
		weapon->sprite = gf2d_sprite_load_all("images/revolver-sprite.png",
			32,
			32,
			1,
			0);
		data->damage = 6;
		data->range = -1;
		data->firerate = 1;
		data->projectileVelocity = 1;
		break;
	case Knife:
		weapon->sprite = gf2d_sprite_load_all("images/knive-sprite.png",
			32,
			32,
			1,
			0);
		data->damage = 2;
		data->range = 0.5;
		data->firerate = 0.75;
		data->projectileVelocity = 0;
		break;
	}

	return weapon;
}

void weapon_free(Entity* ent);
void weapon_update(Entity* self);
void weapon_think(Entity* self);