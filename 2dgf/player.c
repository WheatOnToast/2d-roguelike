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
#include "metronome.h"
#include "gfc_audio.h"
#include "powerup.h"

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

Entity* player_new(Vector2D position, int playerType)
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
    ent->shape = gfc_shape_rect(32, 32, 64, 64);// shape position becomes offset from entity position, in this case zero
	ent->body.shape = &ent->shape;
	ent->velocity.x = 4;
	ent->velocity.y = 4;
	ent->body.worldclip = 1;
	ent->body.team = 1;
	ent->weaponType = 0;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(-32, -32);
    ent->speed = 50;
	ent->playerType = playerType;
	ent->selected = 0;
	ent->powerUpCount = 0;
	ent->AI = 1;
	thePlayer = ent;
	sj_free(json);
	return ent;
}


Vector2D player_get_position(Entity* self)
{
	Vector2D v = { 0 };
	if (!self)return v;
	return self->position;
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

int numOfBullets = 1;
int playerHealth = 1;
int playerAImove = 1;

void player_think(Entity* self)
{


	Vector2D dir;
	dir.x = 30;
	dir.y = 30;
	Vector2D camera;
	Vector2D m = { 0 };
	Vector2D walk = { 0 };
	if (!self) return;



	if (gfc_input_command_pressed("switch"))
	{

		if (self->selected == 1)
		{
			self->selected = 0;
		}
		else if (self->selected == 0)
		{
			self->selected = 1;
		}
	}
	if (gfc_input_command_pressed("powerup"))
	{
		if (powerup_get_powerUpCount() == 1) {
			self->speed += 50;
			powerup_set_powerUpCount(0);
		}
		if (powerup_get_powerUpCount() == 2) {
			int pHealth = projectile_getHealth;
			projectile_setHealth(pHealth + 20);
			powerup_set_powerUpCount(0);
		}
		if (powerup_get_powerUpCount() == 3) {
			int playerHealth = player_getHealth();
			if (playerHealth < 5) {
				player_setHealth(playerHealth + 1);
			}
			powerup_set_powerUpCount(0);
		}
		if (powerup_get_powerUpCount() == 4) {
			int bulletsNum = player_getNumOfBullets();
			player_setNumOfBullets(bulletsNum + 1);
			powerup_set_powerUpCount(0);
		}
		if (powerup_get_powerUpCount() == 5) {
			Entity* ent_list = entity_get_list();
			for (int i = 0; i < 1024; i++) {
				if (ent_list[i].enemyType > 0) {
					entity_free(&ent_list[i]);
				}
			}
			powerup_set_powerUpCount(0);
		}
	}
	if (gfc_input_command_pressed("gunchange"))
	{
		int nextNum = self->weaponType + 1;
		if (nextNum > 1)
		{
			self->weaponType = 0;
			if (self->playerType == 0 && self->selected) {
				self->sprite = gf2d_sprite_load_all(
					"images/britten_yellow.png",
					64,
					64,
					16,
					0);
			}
			else if(self->playerType == 1 && self->selected) {
				self->sprite = gf2d_sprite_load_all(
					"images/britten_red.png",
					64,
					64,
					16,
					0);
			}
		}
		else {
			self->weaponType++;
			if (self->playerType == 0 && self->selected) {
				self->sprite = gf2d_sprite_load_all(
					"images/britten_green.png",
					64,
					64,
					16,
					0);
			}
			else if (self->playerType == 1 && self->selected) {
				self->sprite = gf2d_sprite_load_all(
					"images/britten_blue.png",
					64,
					64,
					16,
					0);
			}
		}

		gui_set_gun(self->weaponType);
		self->numOfWeaponSwitches++;
	}

	if (metronome_isBeatTrue() != 1) {
		playerAImove = 1;
	}

	if (self->selected == 0 && metronome_isBeatTrue() && playerAImove == 1 && self->AI == 1) {
		playerAImove = 0;
		int playerMove = rand() % 4;
		switch (playerMove) {
		case 0:
			self->position.x += 2;
			walk.x += 1;
			break;
		case 1:
			self->position.x -= 2;
			walk.x = -1;
			break;
		case 2:
			self->position.y += 2;
			walk.y += 1;
			break;
		case 3:
			self->position.y -= 2;
			walk.y = -1;
			break;
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

	if (self->selected == 1 && metronome_isBeatTrue())
	{
		camera_center_at(self->position);
		if (gfc_input_command_pressed("walkleft"))
		{
			self->shootRight = 0;
			walk.x = -1;
			self->position.x -= self->speed;
			projectile_setSize(4);
		}
		if (gfc_input_command_pressed("walkright"))
		{
			self->shootRight = 1;
			walk.x += 1;
			self->position.x += self->speed;
			projectile_setSize(4);
		}
		if (gfc_input_command_pressed("walkup"))
		{
			walk.y = -1;
			self->position.y -= self->speed;
			projectile_setSize(4);
		}
		if (gfc_input_command_pressed("walkdown"))
		{
			walk.y += 1;
			self->position.y += self->speed;
			projectile_setSize(4);
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
	else {
		projectile_setSize(1);
	}

	if (self->selected == 1 &&  gfc_input_command_pressed("attack"))
	{
		int bCreateY = self->position.y + 64;
		int bCreateX = self->position.x + 64;
		if (self->weaponType == 0 && self->playerType == 0) //player 0 shoot bullet right
		{
			for (int i = 0; i < numOfBullets; i++) {
				int y = bCreateY + (i*32);
				create_projectile_right(vector2d(bCreateX, y));
				self->numOfBulletsShot++;
			}
		}
		else if (self->weaponType == 1 && self->playerType == 0) //player 0 shoot bullet left
		{
			for (int i = 0; i < numOfBullets; i++) {
				int y = bCreateY + (i * 32);
				create_projectile_left(vector2d(bCreateX, y));
				self->numOfBulletsShot++;
			}
		}

		if (self->weaponType == 0 && self->playerType == 1) { //player 1 shoot bullet up
			self->sprite = gf2d_sprite_load_all(
				"images/britten_blue.png",
				64,
				64,
				16,
				0);
			for (int i = 0; i < numOfBullets; i++) {
				int x = bCreateX + (i * 32);
				create_projectile_up(vector2d(x, bCreateY));
				self->numOfBulletsShot += 1;
			}
		}
		else if (self->weaponType == 1 && self->playerType == 1) { //player 1 shoot bullet down
			self->sprite = gf2d_sprite_load_all(
				"images/britten_red.png",
				64,
				64,
				16,
				0);
			for (int i = 0; i < numOfBullets; i++) {
				int x = bCreateX + (i * 32);
				create_projectile_down(vector2d(x, bCreateY));
				self->numOfBulletsShot += 1;
			}
			
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

int player_getNumOfBullets() {
	return numOfBullets;
}
void player_setNumOfBullets(int num) {
	numOfBullets = num;
}

int player_getHealth() {
	return playerHealth;
}
void player_setHealth(int num) {
	playerHealth = num;
}