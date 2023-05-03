#include "metronome.h"
#include "../2dgf/redEnemy.h"
#include "../2dgf/blueEnemy.h"
#include "../2dgf/greenEnemy.h"
#include "../2dgf/yellowEnemy.h"
#include "../2dgf/whiteEnemy.h"
#include "../2dgf/projectile.h"
float counter = 0;

void metronome_think(Entity* self);
void metronome_free(Entity* self);
void metronome_enemySpawn();
Entity* metronome;

Entity* metronome_new(Vector2D position, int BPM) {
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->BPM = BPM;
	ent->think = metronome_think;
	ent->beatTrue = 0;
	vector2d_copy(ent->position, position);
	metronome = ent;
	return ent;
}

int metronome_isBeatTrue() {
	return metronome->beatTrue;
}
void metronome_enemySpawn() {
	int enemyToSpawn = rand() % 4;
	switch (enemyToSpawn) {
		case 0:
			redEnemy_new(vector2d(544, 500));
			break;
		case 1:
			blueEnemy_new(vector2d(544, 100));
			break;
		case 2:
			greenEnemy_new(vector2d(100, 320));
			break;
		case 3:
			yellowEnemy_new(vector2d(800, 320));
			break;
		
	}

	enemyToSpawn = rand() % 20;
	if (enemyToSpawn == 0) {
		whiteEnemy_new(vector2d(544, 320));
	}
}

void metronome_think(Entity* self) {
		counter += 0.01;
		float timedBeat = (self->BPM/240.0);
		if (counter > timedBeat) {
			self->beatTrue = 1;

			counter = 0;
			if (rand() % 5 == 0) {
				metronome_enemySpawn();
			}
		}
		else if(counter > timedBeat - (timedBeat/4)){
			self->beatTrue = 0;
		}
}

void metronome_free(Entity* self)
{
	if (!self)return;
	entity_free(self);
}