#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "camera.h"
#include "randEnemy.h"

void randEnemy_think(Entity *self);

Entity* randEnemy_new(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)return;
	ent->sprite = gf2d_sprite_load_all(
		"images/img_randSprite.png",
		16,
		16,
		4,
		0);
	ent->think = randEnemy_think;
	ent->maxFrame = 4;

	ent->shape = gfc_shape_circle(0, 0, 20);// shape position becomes offset from entity position, in this case zero
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.team = 1;

	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(0, 0);
	ent->speed = 2.5;
	return ent;
}

void randEnemy_think(Entity* self) 
{
	Vector2D m, dir, camera;
	int mx, my;
	if (!self)return;
	camera = camera_get_position();
	SDL_GetMouseState(&mx, &my);
	m.x = mx;
	m.y = my;
	vector2d_add(m, m, camera);
	vector2d_sub(dir, m, self->position);
	if (vector2d_magnitude_compare(dir, 10) > 0) 
	{
		vector2d_set_magnitude(&dir, self->speed);
		vector2d_copy(self->velocity, dir);
	}
	else
	{
		vector2d_clear(self->velocity);
	}

	//camera_center_at(self->position);
}

/*eol@eof*/