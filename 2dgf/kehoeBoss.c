#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "camera.h"
#include "randEnemy.h"
#include "player.h"
#include "metronome.h"
#include "projectile.h"
#include "gfc_audio.h"

void kehoe_think(Entity* self);

Entity* kehoe_new(Vector2D position)
{
	Sound* mainSong = gfc_sound_load("audio/kehoeCore.mp3", 0.5, 0);
	gfc_sound_play(mainSong, 999, 0.5, -1, -1);

	Entity* ent;
	ent = entity_new();
	if (!ent)return;
	ent->sprite = gf2d_sprite_load_all(
		"images/kehoeBoss.png",
		128,
		256,
		16,
		0);
	ent->think = kehoe_think;
	ent->maxFrame = 4;

	ent->shape = gfc_shape_rect(0, 0, 128, 256);// shape position becomes offset from entity position, in this case zero
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

Vector2D kehoe_get_position(Entity* self)
{
	Vector2D v = { 0 };
	if (!self)return v;
	return self->position;
}

void kehoe_think(Entity* self)
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

	if (metronome_isBeatTrue())
	{
		create_projectile_right(vector2d(self->position.x+128, self->position.y+128));
		create_projectile_left(vector2d(self->position.x, self->position.y + 128));
	}
	else
	{
		vector2d_clear(self->velocity);
	}


	if (self->health <= 0) {
		entity_free(self);
	}

}