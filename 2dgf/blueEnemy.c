#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "camera.h"
#include "randEnemy.h"
#include "player.h"
#include "metronome.h"

void blueEnemy_think(Entity* self);

Entity* blueEnemy_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return;
	ent->sprite = gf2d_sprite_load_all(
		"images/blueEnemy.png",
		64,
		64,
		16,
		0);
	ent->think = blueEnemy_think;
	ent->maxFrame = 4;

	ent->shape = gfc_shape_circle(0, 0, 16);// shape position becomes offset from entity position, in this case zero
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.team = 1;

	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(32, 32);
	ent->speed = 2.5;
	ent->enemyType = 2;
	ent->health = 1;
	return ent;
}

Vector2D blueEnemy_get_position(Entity* self)
{
	Vector2D v = { 0 };
	if (!self)return v;
	return self->position;
}

void blueEnemy_think(Entity* self)
{
	if (metronome_isBeatTrue())
	{
		self->position.y += 1;
	}
	else
	{
		vector2d_clear(self->velocity);
	}


	if (self->health <= 0) {
		entity_free(self);
	}


	if (self->health <= 0) {
		entity_free(self);
	}

}

/*eol@eof*/