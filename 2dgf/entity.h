#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gfc_primitives.h"

#include "body.h"
#include "actor.h"

typedef struct Entity_S {
	Bool _inuse;
	Sprite *sprite;
	TextLine name;
	Actor* actor;
	Action* action;
	float frame;
	int maxFrame;
	float rotation;
	float speed;
	Vector2D drawOffset;
	Shape shape; //collision shape
	Body body; //instance for 2D collisions
	Box bounds;
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;
	int health;
	int damage;
	Bool canJump;
	Bool canMove;
	int weaponType;
	Bool shootRight;
	struct Entity_S* parent;    /**<entity that spawned this one*/

	//====
	//game statistics
	int numOfJumps;
	int numOfWeaponSwitches;
	int numOfBulletsShot;
	//====

	float cooldown;
	void (*think)(struct Entity *self);
	int (*update)(struct Entity *self);
	void (*draw)(struct Entity *self);
	void (*free_entity)(struct Entity *self);
	void (*takeDamage)(struct Entity_S* self, float damage, struct Entity_S* inflictor); /**<pointer to the think function*/
	void* data;
}Entity;

/**
 * @brief initialize the internal manager for the entity system
 * @note this will automatically queue up the close function for program exit
 * @param max this is the maximum number of supported entities at a given time
 */
void entity_manager_init(Uint32 max);

/**
 * @brief alloate an initialize a new entity
 * @return NULL if there are no entities left, an empty entity otherwise
 */
Entity* entity_new();

/**
 * @brief free a previously allocated entity
 * @param ent the entity to free
 */
void entity_free(Entity* ent);

/**
 * @brief free all allocated entities
 */
void entity_free_all();

/**
 * @brief draw all active entities if they have graphics
 */
void entity_draw_all();

/**
 * @brief update all active entities
 */
void entity_update_all();

/**
 * @brief call all the think functions for the entities, if they have one
 */
void entity_think_all();

/**
 * @brief given an entity get its shape in world space
 * @param ent the entity to check
 * @return a shape where its position is set to the world position
 */
Shape entity_get_shape(Entity* ent);

/**
 * @brief given an entity get its shape in world space where it will be after it moves
 * @param ent the entity to check
 * @return a shape where its position + velocity is set to the world position
 */
Shape entity_get_shape_after_move(Entity* ent);
#endif