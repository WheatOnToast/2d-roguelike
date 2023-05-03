#include "simple_logger.h"
#include "gfc_input.h"
#include "gf2d_draw.h"
#include "camera.h"
#include "level.h"
#include "player.h"
#include "powerup.h"

void powerup_think(Entity* self);
void powerup_draw(Entity* self);
void powerup_free(Entity* self);

int powerUpCount = 0;

Entity* powerup_new(Vector2D position) {
	Entity* ent = entity_new();
	if (!ent)return NULL;
	ent->think = powerup_think;
	ent->draw = powerup_draw;
	ent->sprite = ent->sprite = gf2d_sprite_load_all(
		"images/bullet_yellow.png",
		32,
		32,
		16,
		0);
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(8, 8);
	ent->shape = gfc_shape_circle(8, 4, 16);
	return ent;
}

void powerup_draw(Entity* self) {
	Vector2D drawPosition, camera;
	if (!self)return;
	camera = camera_get_draw_offset();
	vector2d_add(drawPosition, self->position, camera);
	gf2d_draw_circle(drawPosition, 20, GFC_COLOR_YELLOW);
}

void powerup_free(Entity* self) {
	if (!self)return;
	entity_free(self);
}

void powerup_think(Entity* self) {
	Entity* ent_list = entity_get_list();
	for (int i = 0; i < 1024; i++) {
		if (ent_list[i].selected == 1) {

			Vector2D a = player_get_position(&ent_list[i]);
			Vector2D b = self->position;

			if (vector2d_magnitude_between(a, b) < 120) {
				slog("%s", "got powerup");
				if (powerUpCount < 5) {
					powerUpCount++;
				}
				powerup_free(self);
			}
			break;
		}
	}
}

int powerup_get_powerUpCount() {
	return powerUpCount;
}
void powerup_set_powerUpCount(int num) {
	powerUpCount = num;
}