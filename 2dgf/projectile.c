#include "simple_logger.h"

#include "gf2d_draw.h"
#include "camera.h"
#include "collision.h"

#include "level.h"
#include "projectile.h"
#include "player.h"
#include "powerup.h"

#include "../2dgf/redEnemy.h"
#include "../2dgf/blueEnemy.h"
#include "../2dgf/greenEnemy.h"
#include "../2dgf/yellowEnemy.h"

void projectile_think(Entity* self);
void projectile_draw(Entity* self);
void projectile_update(Entity* self);

int projectile_health = 10;
int projectile_size = 1;
Entity* create_projectile_right(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return;
    ent->sprite = gf2d_sprite_load_all(
        "images/bullet_yellow.png",
        32,
        32,
        16,
        0);
    ent->maxFrame = 1;
    ent->think = projectile_think_right;
    ent->update = projectile_update;
    ent->draw = projectile_draw;
    ent->shape = gfc_shape_circle(8 * projectile_size, 4 * projectile_size, 16 * projectile_size);// shape position becomes offset from entity position, in this case zero
    ent->body.shape = &ent->shape;
    ent->body.worldclip = 1;
    ent->body.team = 1;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(8, 8);
    ent->speed = 2.5;
    ent->health = projectile_health;
    ent->bulletType = 3;
    return ent;
}

Entity* create_projectile_left(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return;
    ent->sprite = gf2d_sprite_load_all(
        "images/bullet_green.png",
        32,
        32,
        16,
        0);
    ent->maxFrame = 1;
    ent->think = projectile_think_left;
    ent->update = projectile_update;
    ent->draw = projectile_draw;
    ent->shape = gfc_shape_circle(8 * projectile_size, 4 * projectile_size, 16 * projectile_size);// shape position becomes offset from entity position, in this case zero
    ent->body.shape = &ent->shape;
    ent->body.worldclip = 1;
    ent->body.team = 1;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(8 * projectile_size, 8 * projectile_size);
    ent->speed = 2.5;
    ent->health = projectile_health;
    ent->bulletType = 2;
    return ent;
}

Entity* create_projectile_up(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return;
    ent->sprite = gf2d_sprite_load_all(
        "images/bullet_blue.png",
        32,
        32,
        16,
        0);
    ent->maxFrame = 1;
    ent->think = projectile_think_up;
    ent->update = projectile_update;
    ent->draw = projectile_draw;
    ent->shape = gfc_shape_circle(8 * projectile_size, 4 * projectile_size, 16 * projectile_size);
    ent->body.shape = &ent->shape;
    ent->body.worldclip = 1;
    ent->body.team = 1;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(8 * projectile_size, 8 * projectile_size);
    ent->speed = 2.5;
    ent->health = projectile_health;
    ent->bulletType = 0;
    return ent;
}

Entity* create_projectile_down(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return;
    ent->sprite = gf2d_sprite_load_all(
        "images/bullet_red.png",
        32,
        32,
        16,
        0);
    ent->maxFrame = 1;
    ent->think = projectile_think_down;
    ent->update = projectile_update;
    ent->draw = projectile_draw;
    ent->shape = gfc_shape_circle(8 * projectile_size, 4 * projectile_size, 16 * projectile_size);
    ent->body.shape = &ent->shape;
    ent->body.worldclip = 1;
    ent->body.team = 1;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(8 * projectile_size, 8 * projectile_size);
    ent->speed = 2.5;
    ent->health = projectile_health;
    ent->bulletType = 1;
    return ent;
}
Entity* projectile_new(Entity* parent, Vector2D position, Vector2D dir, float speed, float damage, const char* actor)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->actor = gf2d_actor_load(actor);
    ent->sprite = gf2d_sprite_load_all(
        "images/bullet.png",
        32,
        32,
        1,
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
    ent->health = 1000;
    ent->damage = damage;
    
    level_add_entity(level_get_active_level(), ent);
    return ent;
}

float mf;
void projectile_draw(Entity* self)
{
    Vector2D drawPosition, camera;
    if (!self)return;
    entity_draw();

    camera = camera_get_draw_offset();
    vector2d_add(drawPosition, self->position, camera);
    gf2d_draw_circle(drawPosition, 1000, GFC_COLOR_YELLOW);
    mf+=0.1;
    if (mf >= 16.0)mf = 0;
    Sprite* bullet = { 0 };
    if (self->bulletType == 0) {
        Sprite* bullet = gf2d_sprite_load_all("images/bullet_red.png", 32 , 32 , 0, 0);
    }
    else if (self->bulletType == 1) {
        Sprite* bullet = gf2d_sprite_load_all("images/bullet_green.png", 32 , 32 , 0, 0);
    }
    else if (self->bulletType == 2) {
        Sprite* bullet = gf2d_sprite_load_all("images/bullet_blue.png", 32 , 32 , 0, 0);
    }
    else if (self->bulletType == 3){
        Sprite* bullet = gf2d_sprite_load_all("images/bullet_yellow.png", 32 , 32 , 0, 0);
    }

    int mf = 0;

    gf2d_sprite_draw(
        bullet,
        self->position,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)mf);
}




void projectile_think_left(Entity* self)
{
    Entity* ent_list = entity_get_list();
    for (int i = 0; i < 1024; i++) {
        if (ent_list[i].enemyType == 3 || ent_list[i].enemyType == 5) {


            Vector2D a = greenEnemy_get_position(&ent_list[i]);
            Vector2D b = self->position;

            if (vector2d_magnitude_between(a, b) < 150) {
          
                powerup_new(vector2d(ent_list[i].position.x, ent_list[i].position.y));
                ent_list[i].health--;
            }
            break;
        }
    }

    if (!self)return;
    self->health--;
    self->position.x -= 10;
    if (self->health <= 0)entity_free(self);
}

void projectile_think_right(Entity* self)
{
    Entity* ent_list = entity_get_list();
    for (int i = 0; i < 1024; i++) {
        if (ent_list[i].enemyType == 4 || ent_list[i].enemyType == 5) {


            Vector2D a = yellowEnemy_get_position(&ent_list[i]);
            Vector2D b = self->position;

            if (vector2d_magnitude_between(a, b) < 150) {
                
                powerup_new(vector2d(ent_list[i].position.x, ent_list[i].position.y));
                ent_list[i].health--;
            }
            break;
        }
    }

    if (!self)return;
    self->health--;
    self->position.x += 10;
    if (self->health <= 0)entity_free(self);
}

void projectile_think_up(Entity* self)
{
    Entity* ent_list = entity_get_list();
    for (int i = 0; i < 1024; i++) {
        if (ent_list[i].enemyType == 2 || ent_list[i].enemyType == 5) {


            Vector2D a = blueEnemy_get_position(&ent_list[i]);
            Vector2D b = self->position;

            if (vector2d_magnitude_between(a, b) < 150) {
             
                powerup_new(vector2d(ent_list[i].position.x, ent_list[i].position.y));
                ent_list[i].health--;
            }
            break;
        }
    }

    if (!self)return;
    self->health--;
    self->position.y -= 10;
    if (self->health <= 0)entity_free(self);
}

void projectile_think_down(Entity* self)
{

    if (!self)return;

    Entity* ent_list = entity_get_list();
    for (int i = 0; i < 1024; i++) {
        if (ent_list[i].enemyType == 1 || ent_list[i].enemyType == 5) {

            
            Vector2D a = redEnemy_get_position(&ent_list[i]);
            Vector2D b = self->position;
            
            if (vector2d_magnitude_between(a, b) < 150) {
              
                powerup_new(vector2d(ent_list[i].position.x, ent_list[i].position.y));
                ent_list[i].health--;
            }
            break;
        }
    }

    self->health--;
    self->position.y += 10;
    if (self->health <= 0)entity_free(self);
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

int projectile_getHealth() {
    return projectile_health;
}

void projectile_setHealth(int num) {
    projectile_health = num;
}

int projectile_getSize() {
    return projectile_size;
}

void projectile_setSize(int size) {
    projectile_size = size;
}

/*eol@eof*/