#include "simple_logger.h"

#include "gfc_input.h"
#include "gf2d_draw.h"
#include "camera.h"

#include "level.h"
#include "player.h"
#include "projectile.h"
#include "weapon.h"
#include "randEnemy.h"
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
	ent = entity_new();
	if (!ent)return NULL;
    gfc_line_cpy(ent->name, "player");
	ent->sprite = gf2d_sprite_load_all(
		"images/img_randSprite.png",
		16,
		16,
		4,
		0);
	ent->maxFrame = 4;
    ent->think = player_think;
    ent->draw = player_draw;
    ent->free_entity = player_free;
    ent->shape = gfc_shape_circle(0, 0, 20);// shape position becomes offset from entity position, in this case zero
	ent->body.shape = &ent->shape;
	ent->velocity.x = 4;
	ent->velocity.y = 4;
	ent->body.worldclip = 1;
	ent->body.team = 1;
	
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(0, 0);
    ent->speed = 2.5;
	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data)
	{
		data->health = 3;
		data->attack = 1;
		data->defense = 1;
		data->speed = 2;
	}

	thePlayer = ent;
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
	gf2d_draw_circle(drawPosition, 10, GFC_COLOR_YELLOW);
}

void player_think(Entity* self)
{

	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) != 0) { //if touching a platform
		self->canJump = 1;
	}
	else if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 0) { //if not touching a platform
		self->position.y += 2.5;
		//self->velocity.y += 2.5;
	}
	if (self->position.y > 800) {
		self->position.y += 2.5;
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
	if (self->canMove == 1)
	{
		camera_center_at(self->position);
		if (gfc_input_command_down("walkleft"))
		{
			walk.x = -1;
			self->position.x -= 3;
		}
		if (gfc_input_command_down("walkright"))
		{
			
			walk.x += 1;
			self->position.x += 3;
		}
		if (gfc_input_command_pressed("walkup"))
		{
			self->canJump = 0;
			self->position.y -= 100;
		}
		if (gfc_input_command_pressed("attack"))
		{
			int bCreate = self->position.y - 35;
			create_projectile(vector2d(self->position.x, bCreate));
			player_attack(self);
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