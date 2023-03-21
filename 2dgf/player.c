#include "simple_logger.h"

#include "gfc_input.h"
#include "gf2d_draw.h"
#include "camera.h"

#include "level.h"
#include "player.h"
#include "projectile.h"
#include "weapon.h"
#include "randEnemy.h"
#include "gui.h"

void player_think(Entity* self);
void player_draw(Entity* self);
void player_free(Entity* self);

static Entity *thePlayer = NULL;

typedef struct
{
    Uint32 health;
	Uint32 attack;
	Uint32 defense;
	Uint32 speed;
}PlayerData;

Entity* player_new(Vector2D position)
{
	PlayerData* data;
	Entity* ent;
	SJson* json, * player;
	int health, defense, attack, speed;
	const char *name, *sprite;

	ent = entity_new();
	if (!ent)return NULL;

	// ----==== JSON File Load ====----
	json = sj_load("config/player.json");
	if (!json) {
		slog("Can't find player JSON");
		return NULL;
	}
	//finding "player" key
	player = sj_object_get_value(json, "player");

	//extracting values from "player"
	name = sj_object_get_value_as_string(player, "name");
	sprite = sj_object_get_value_as_string(player, "sprite");
	sj_object_get_value_as_int(player, "health", &health);
	sj_object_get_value_as_int(player, "defense", &defense);
	sj_object_get_value_as_int(player, "attack", &attack);
	sj_object_get_value_as_int(player, "speed", &speed);

	//inserting JSON values into player entity and data
    gfc_line_cpy(ent->name, name);
	ent->sprite = gf2d_sprite_load_all(
		sprite,
		64,
		64,
		16,
		0);
	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data)
	{
		data->health = health;
		data->attack = attack;
		data->defense = defense;
		data->speed = speed;
	}
	// ----=============================----

	ent->maxFrame = 0;
    ent->think = player_think;
    ent->draw = player_draw;
    ent->free_entity = player_free;
    ent->shape = gfc_shape_rect(2, 2, 2, 4);// shape position becomes offset from entity position, in this case zero
	ent->body.shape = &ent->shape;
	ent->velocity.x = 4;
	ent->velocity.y = 4;
	ent->body.worldclip = 1;
	ent->body.team = 1;
	ent->weaponType = 0;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(0, 0);
    ent->speed = 2.5;


	thePlayer = ent;
	sj_free(json);
	return ent;
}

void player_attack(Entity* self)
{
	Vector2D dir;

	PlayerData* data;
	WeaponData* wData;
	if ((!self) || (!self->data))return;
	data = self->data;
	dir = vector2d_from_angle(30);
	projectile_new(self, thePlayer->position, dir, 5, 10, "actors/plasma_bolt.actor");
	self->cooldown = 5;
	if ((!self->action) || (gfc_strlcmp(self->action->name, "shoot") != 0))
	{
		self->action = gf2d_actor_set_action(self->actor, "shoot", &self->frame);
	}
}

Vector2D player_get_position()
{
	Vector2D v = { 0 };
	if (!thePlayer)return v;
	return thePlayer->position;
}

Entity* player_get()
{
	return thePlayer;
}

void player_draw(Entity* self)
{
	Vector2D drawPosition, camera;
	if (!self)return;
	camera = camera_get_draw_offset();
	vector2d_add(drawPosition, self->position, camera);
	gf2d_draw_circle(drawPosition, 20, GFC_COLOR_YELLOW);
}

void player_think(Entity* self)
{

	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) != 0) { //if touching a platform
		self->canJump = 1;
	}
	else if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 0) { //if not touching a platform
		self->position.y += 3;
		//self->velocity.y += 2.5;
	}
	if (self->position.y > 700) {
		self->position.y += 3;
	}

	Vector2D dir;
	dir.x = 30;
	dir.y = 30;
	Vector2D camera;
	Vector2D m = { 0 };
	Vector2D walk = { 0 };
	if (!self) return;

	
	if (gfc_input_command_pressed("switch"))
	{

		if (self->canMove == 1)
		{
			self->canMove = 0;
		}
		else if (self->canMove == 0)
		{
			self->canMove = 1;
		}
	}
	if (gfc_input_command_pressed("gunchange"))
	{
		int nextNum = self->weaponType + 1;
		if (nextNum > 4)
		{
			self->weaponType = 0;
		}
		else {
			self->weaponType++;
		}

		gui_set_gun(self->weaponType);
		self->numOfWeaponSwitches++;
	}
	if (self->canMove == 1)
	{
		camera_center_at(self->position);
		if (gfc_input_command_down("walkleft"))
		{
			self->shootRight = 0;
			walk.x = -1;
			self->position.x -= 3;
		}
		if (gfc_input_command_down("walkright"))
		{
			self->shootRight = 1;
			walk.x += 1;
			self->position.x += 3;
		}
		if (gfc_input_command_pressed("walkup"))
		{
			self->canJump = 0;
			self->position.y -= 100;

			self->numOfJumps++;
		}
		if (gfc_input_command_pressed("attack"))
		{
			int bCreate = self->position.y - 50;
			if (self->weaponType == 0 && self->shootRight == 1) //WEAPON 0
			{
				create_projectile_right(vector2d(self->position.x, bCreate));
				self->numOfBulletsShot++;
			}
			else if (self->weaponType == 0 && self->shootRight == 0)
			{
				create_projectile_left(vector2d(self->position.x, bCreate));
				self->numOfBulletsShot++;
			}

			if (self->weaponType == 1) { //WEAPON 1
				create_projectile_right(vector2d(self->position.x, bCreate));
				create_projectile_left(vector2d(self->position.x, bCreate));
				self->numOfBulletsShot += 2;
			}

			if (self->weaponType == 2) { //WEAPON 2
				create_projectile_right(vector2d(self->position.x, bCreate));
				create_projectile_left(vector2d(self->position.x, bCreate));
				create_projectile_up(vector2d(self->position.x, bCreate));
				self->numOfBulletsShot += 3;
			}

			if (self->weaponType == 3) { //WEAPON 3
				create_projectile_up(vector2d(self->position.x, bCreate));
				create_projectile_down(vector2d(self->position.x, bCreate));
				self->numOfBulletsShot += 2;
			}

			if (self->weaponType == 4) { //WEAPON 4
				create_projectile_right(vector2d(self->position.x, bCreate));
				create_projectile_left(vector2d(self->position.x, bCreate));
				create_projectile_up(vector2d(self->position.x, bCreate));
				create_projectile_down(vector2d(self->position.x, bCreate));
				self->numOfBulletsShot += 4;
			}
		}
	
		if ((walk.x) || (walk.y))
		{
			vector2d_normalize(&walk);
			vector2d_scale(walk, walk, self->speed);
			vector2d_copy(self->velocity, walk);
		}
		else
		{
			vector2d_clear(self->velocity);
		}
	}

}

void player_free(Entity* self)
{
	if (!self)return;
	thePlayer = NULL;
}

Uint8 entity_clip_with_player(Entity* ent)
{
	return (gfc_shape_overlap(ent->shape, thePlayer->shape));
}