#include "simple_logger.h"

#include "gf2d_draw.h"
#include "camera.h"
#include "collision.h"

#include "level.h"
#include "projectile.h"

void projectile_think(Entity* self);
void projectile_draw(Entity* self);
void projectile_update(Entity* self);


Entity* create_projectile(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return;
    ent->sprite = gf2d_sprite_load_all(
        "images/plasma_bolt.png",
        16,
        16,
        4,
        0);
    ent->maxFrame = 4;
    ent->think = projectile_think;
    ent->update = projectile_update;
    ent->draw = projectile_draw;
    ent->shape = gfc_shape_circle(0, 0, 50);// shape position becomes offset from entity position, in this case zero
    ent->body.shape = &ent->shape;
    ent->body.worldclip = 1;
    ent->body.team = 1;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(0, 0);
    ent->speed = 2.5;
    return ent;
}

Entity* projectile_new(Entity* parent, Vector2D position, Vector2D dir, float speed, float damage, const char* actor)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->actor = gf2d_actor_load(actor);
    ent->sprite = gf2d_sprite_load_all(
        "images/plasma_bolt.png",
        16,
        16,
        4,
        0);
    ent->action = gf2d_actor_get_action_by_name(ent->actor, "default");
    //ent->think = projectile_think;
    ent->update = projectile_update;
    ent->draw = projectile_draw;
    ent->shape = gfc_shape_circle(0, 0, 5);// shape position becomes offset from entity position, in this case zero
    ent->body.shape = &ent->shape;
    if (parent)
    {
        ent->body.team = parent->body.team;
    }
    vector2d_copy(ent->body.position, position);
    vector2d_normalize(&dir);
    vector2d_scale(ent->body.velocity, dir, speed);
    ent->rotation += GFC_HALF_PI;
    ent->speed = speed;
    ent->health = 100000000;
    ent->damage = damage;
    level_add_entity(level_get_active_level(), ent);
    return ent;
}

void projectile_draw(Entity* self)
{
    Vector2D drawPosition, camera;
    if (!self)return;
    camera = camera_get_draw_offset();
    vector2d_add(drawPosition, self->position, camera);
    gf2d_draw_circle(drawPosition, 1000, GFC_COLOR_YELLOW);
}

void projectile_think(Entity* self)
{
    //if (!self)return;
    //self->health--;
    self->position.x += 10;
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) != 0) { //if touching a platform
        entity_free(self);
    }
    //if (self->health <= 0)entity_free(self);
}

void projectile_update(Entity* self)
{
    int i, c;
    Entity* other;
    Collision* collide;
    Shape shape = { 0 };
    List* collision;
    CollisionFilter filter = { 0 };
    if (!self)return;
    //TODO hit stuff
    gfc_shape_copy(&shape, self->shape);
    gfc_shape_move(&shape, self->position);

    filter.worldclip = 1;
    filter.cliplayer = 1;
    if (self->parent)
    {
        filter.ignore = &self->parent->body;         /**<this body will specifically be skipped in checks*/
    }
    collision = gf2d_collision_check_space_shape(level_get_space(level_get_active_level()), shape, filter);

    if (!collision)return;
    c = gfc_list_get_count(collision);
    for (i = 0; i < c; i++)
    {
        collide = gfc_list_get_nth(collision, i);
        if (!collide)continue;
        if (collide->body == NULL)continue;
        if (!collide->body->data)continue;
        other = collide->body->data;
        if (other->takeDamage)other->takeDamage(other, self->damage, self);
    }
    gf2d_collision_list_free(collision);
    entity_free(self);
}

/*eol@eof*/