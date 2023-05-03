#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "camera.h"
#include "randEnemy.h"
#include "player.h"
#include "metronome.h"
#include "powerup.h"

void whiteEnemy_think(Entity* self);

Entity* whiteEnemy_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return;
	ent->sprite = gf2d_sprite_load_all(
		"images/whiteEnemy.png",
		64,
		64,
		16,
		0);
	ent->think = whiteEnemy_think;
	ent->maxFrame = 4;

	ent->shape = gfc_shape_circle(0, 0, 16);// shape position becomes offset from entity position, in this case zero
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.team = 1;

	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(32, 32);
	ent->speed = 2.5;
	ent->enemyType = 5;
	ent->health = 1;
	return ent;
}

Vector2D whiteEnemy_get_position(Entity* self)
{
	Vector2D v = { 0 };
	if (!self)return v;
	return self->position;
}

void whiteEnemy_think(Entity* self)
{

	Vector2D m, dir, camera, playerVec;

	if (self->health <= 0) {
		powerup_new(vector2d(self->position.x - 32,self->position.y - 32));
		powerup_new(vector2d(self->position.x + 32, self->position.y - 32));
		powerup_new(vector2d(self->position.x - 32, self->position.y + 32));
		powerup_new(vector2d(self->position.x + 32, self->position.y + 32));
		entity_free(self);
	}

}

/*eol@eof*/