#include <SDL.h>
#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_list.h"
#include "gfc_audio.h"
#include "gfc_input.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "../2dgf/level.h"
#include "../2dgf/camera.h"
#include "../2dgf/entity.h"
#include "../2dgf/randEnemy.h"
#include "../2dgf/player.h"
#include "../2dgf/gui.h"
int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Entity *ent;
    int mx,my;
    Level* level;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1136,
        656,
        1136,
        656,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    gfc_input_init("config/input.cfg");

    /*entity manager init*/
    entity_manager_init(1024);

    SDL_ShowCursor(SDL_DISABLE);

    /*demo setup*/
    level = level_load("config/test.level");
    level_set_active_level(level);

    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    //ent = randEnemy_new(vector2d(100, 100));
    Entity* player1 = player_new(vector2d(600, 570));
    Entity* player2 = player_new(vector2d(1400, 100));
    player1->canMove = 1;
    player2->canMove = 0;
    
    /*main game loop*/
    while(!done)
    {
        gui_draw_hud();
        gfc_input_update(); // input update
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        entity_think_all();
        entity_update_all();
        camera_world_snap();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level_get_active_level());
            entity_draw_all();
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
            if (player1->weaponType == 0)
            {
                char a = (char)player1->weaponType;
                char file = "images/weapon0.png";
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/weapon0.png", 64, 64, 16, 0),
                    vector2d(100, 100),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player1->weaponType == 1)
            {
                char a = (char)player1->weaponType;
                char file = "images/weapon0.png";
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/weapon1.png", 64, 64, 16, 0),
                    vector2d(100, 100),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player1->weaponType == 2)
            {
                char a = (char)player1->weaponType;
                char file = "images/weapon0.png";
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/weapon2.png", 64, 64, 16, 0),
                    vector2d(100, 100),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player1->weaponType == 3)
            {
                char a = (char)player1->weaponType;
                char file = "images/weapon0.png";
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/weapon3.png", 64, 64, 16, 0),
                    vector2d(100, 100),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
            else if (player1->weaponType == 4)
            {
                char a = (char)player1->weaponType;
                char file = "images/weapon0.png";
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/weapon4.png", 64, 64, 16, 0),
                    vector2d(100, 100),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
            }
        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());

  
        
    }
    level_free(level);
   //entity_free(ent);
    SJson* json, * src;
    json = sj_load("config/results.json");
    src = sj_copy(json);
    sj_object_insert(src, "totalJumps", sj_new_int(player1->numOfJumps));
    sj_object_insert(src, "totalBulletsShot", sj_new_int(player1->numOfBulletsShot));
    sj_object_insert(src, "totalWeaponSwitches", sj_new_int(player1->numOfWeaponSwitches));
    sj_save(src, "config/results.json");

    return 0;
}

/*eol@eof*/
