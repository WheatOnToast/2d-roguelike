#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "camera.h"
#include "randEnemy.h"
#include "player.h"
#include "metronome.h"
#include "projectile.h"

void boss3_think(Entity* self);

Entity* boss3_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return;
	ent->sprite = gf2d_sprite_load_all(
		"images/boss3.png",
		64,
		128,
		16,
		0);
	ent->think = boss3_think;
	ent->maxFrame = 4;

	ent->shape = gfc_shape_rect(0, 0, 64, 128);// shape position becomes offset from entity position, in this case zero
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.team = 1;

	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(0, 0);
	ent->speed = 2.5;
	ent->enemyType = 1;
	ent->health = 30;
	return ent;
}

Vector2D boss3_get_position(Entity* self)
{
	Vector2D v = { 0 };
	if (!self)return v;
	return self->position;
}

void boss3_think(Entity* self)
{

	Vector2D m, dir, camera, playerVec;
	int px, py;
	if (!self)return;
	camera = camera_get_position();


	Entity* ent_list = entity_get_list();
	for (int i = 0; i < 1024; i++) {
		if (ent_list[i].selected == 1) {
			playerVec = player_get_position(&ent_list[i]);
			break;
		}
	}

	m.x = playerVec.x + 32;
	m.y = playerVec.y + 32;

	vector2d_add(m, m, camera);
	vector2d_sub(dir, m, self->position);

	if (vector2d_magnitude_compare(dir, 10) > 0 && metronome_isBeatTrue())
	{
		create_projectile_up(vector2d(self->position.x + 64, self->position.y-64));
		create_projectile_down(vector2d(self->position.x + 32, self->position.y + 150));
		vector2d_set_magnitude(&dir, 2);
		vector2d_copy(self->velocity, dir);
	}
	else
	{
		vector2d_clear(self->velocity);
	}


	if (self->health <= 0) {
		entity_free(self);
	}


}