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
#include "../2dgf/metronome.h"
#include "../2dgf/powerup.h"
#include "../2dgf/redEnemy.h"
#include "../2dgf/blueEnemy.h"
#include "../2dgf/greenEnemy.h"
#include "../2dgf/yellowEnemy.h"
#include "../2dgf/whiteEnemy.h"
#include "../2dgf/powerup.h"
#include "../2dgf/kehoeBoss.h"
#include "../2dgf/boss2.h"
#include "../2dgf/boss3.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Entity* ent, * player1 = { 0 }, * player2 = { 0 };
    int mx,my;
    Level* level = { 0 };
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1088,
        640,
        1088,
        640,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    gfc_input_init("config/input.cfg");
    

    /*entity manager init*/
    entity_manager_init(1024);

    SDL_ShowCursor(SDL_DISABLE);

    /*audio init*/
    gfc_audio_init(4, 4, 4, 4, 1, 0);

    /*demo setup*/


    sprite = gf2d_sprite_load_image("images/backgrounds/bg_title_screen.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    int gameStart = 0;

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
        if (gfc_input_command_pressed("gameStart1")) {
            entity_free_all();
            if (gameStart > 0) {
                gfc_sound_clear_all();
                gf2d_sprite_clear_all();
                gameStart = 0;
            }
            else {
                gameStart = 1;
            }
        }
        else if (gfc_input_command_pressed("gameStart2")) {
            entity_free_all();
            if (gameStart > 0) {
                gameStart = 0;
            }
            else {
                gameStart = 2;
            }
        }
        else if (gfc_input_command_pressed("gameStart3")) {
            entity_free_all();
            if (gameStart > 0) {
                gameStart = 0;
            }
            else {
                gameStart = 3;
            }
        }
        else if (gfc_input_command_pressed("gameStart4")) {
            entity_free_all();
            if (gameStart > 0) {
                gameStart = 0;
            }
            else {
                gameStart = 4;
            }
        }
        else if (gfc_input_command_pressed("gameStart5")) {
            entity_free_all();
            if (gameStart > 0) {
                gameStart = 0;
            }
            else {
                gameStart = 5;
            }
        }

        if (gameStart == 0) {
            gf2d_sprite_load_image("images/backgrounds/bg_title_screen.png");
        }
        else if (gameStart == 1) {
            gf2d_sprite_clear_all();
            gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
            level = level_load("config/test.level");
            level_set_active_level(level);
            Entity* metronome = metronome_new(vector2d(0, 0), 120);
            player1 = player_new(vector2d(320, 384), 0);
            player2 = player_new(vector2d(400, 324), 1);
            player1->selected = 1;
            player2->selected = 0;
            Sound* mainSong = gfc_sound_load("audio/120bpm.mp3", 0.5, 0);

            /*play song*/
            gfc_sound_play(mainSong, 999, 0.5, -1, -1);
            gameStart = 99;
        }
        else if (gameStart == 2) {
            gf2d_sprite_clear_all();
            gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
            level = level_load("config/test.level");
            level_set_active_level(level);
            Entity* metronome = metronome_new(vector2d(0, 0), 180);
            player1 = player_new(vector2d(320, 384), 0);
            player2 = player_new(vector2d(400, 324), 1);
            player1->selected = 1;
            player2->selected = 0;
            player1->AI = 0;
            player2->AI = 0;
            powerup_new(vector2d(100, 100));

            powerup_new(vector2d(100, 200));
            powerup_new(vector2d(100, 200));

            powerup_new(vector2d(100, 300));
            powerup_new(vector2d(100, 300));
            powerup_new(vector2d(100, 300));

            powerup_new(vector2d(100, 400));
            powerup_new(vector2d(100, 400));
            powerup_new(vector2d(100, 400));
            powerup_new(vector2d(100, 400));

            powerup_new(vector2d(100, 500));
            powerup_new(vector2d(100, 500));
            powerup_new(vector2d(100, 500));
            powerup_new(vector2d(100, 500));
            powerup_new(vector2d(100, 500));
            Sound* mainSong = gfc_sound_load("audio/120bpm.mp3", 0.5, 0);

            /*play song*/
            gfc_sound_play(mainSong, 999, 0.5, -1, -1);
            gameStart = 99;
        }
        else if (gameStart == 3) {
            gf2d_sprite_clear_all();
            gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
            level = level_load("config/test.level");
            level_set_active_level(level);
            Entity* metronome = metronome_new(vector2d(0, 0), 240);
            boss2_new(vector2d(100, 100));
            player1 = player_new(vector2d(320, 384), 0);
            player2 = player_new(vector2d(400, 324), 1);
            player1->selected = 1;
            player2->selected = 0;
            Sound* mainSong = gfc_sound_load("audio/120bpm.mp3", 0.5, 0);

            /*play song*/
            gfc_sound_play(mainSong, 999, 0.5, -1, -1);
            gameStart = 99;
        }
        else if (gameStart == 4) {
            gf2d_sprite_clear_all();
            gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
            level = level_load("config/test.level");
            level_set_active_level(level);
            Entity* metronome = metronome_new(vector2d(0, 0), 240);
            boss3_new(vector2d(100, 100));
            player1 = player_new(vector2d(320, 384), 0);
            player2 = player_new(vector2d(400, 324), 1);
            player1->selected = 1;
            player2->selected = 0;
            Sound* mainSong = gfc_sound_load("audio/120bpm.mp3", 0.5, 0);

            /*play song*/
            gfc_sound_play(mainSong, 999, 0.5, -1, -1);
            gameStart = 99;
        }
        else if (gameStart == 5) {
            gf2d_sprite_clear_all();
            gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
            level = level_load("config/test.level");
            level_set_active_level(level);
            Entity* metronome = metronome_new(vector2d(0, 0), 240);
            kehoe_new(vector2d(460, 250));
            player1 = player_new(vector2d(320, 384), 0);
            player2 = player_new(vector2d(400, 324), 1);
            player1->selected = 1;
            player2->selected = 0;
            Sound* mainSong = gfc_sound_load("audio/120bpm.mp3", 0.5, 0);

            /*play song*/
            gfc_sound_play(mainSong, 999, 0.5, -1, -1);
            gameStart = 99;
        }

        else if (gameStart == 99) {
            //power up bar UI
            if (powerup_get_powerUpCount() == 0) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/powerup_bar_0.png", 320, 64, 1, 0),
                    vector2d(0, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (powerup_get_powerUpCount() == 1) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/powerup_bar_1.png", 320, 64, 1, 0),
                    vector2d(0, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (powerup_get_powerUpCount() == 2) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/powerup_bar_2.png", 320, 64, 1, 0),
                    vector2d(0, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (powerup_get_powerUpCount() == 3) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/powerup_bar_3.png", 320, 64, 1, 0),
                    vector2d(0, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (powerup_get_powerUpCount() == 4) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/powerup_bar_4.png", 320, 64, 1, 0),
                    vector2d(0, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (powerup_get_powerUpCount() == 5) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/powerup_bar_5.png", 320, 64, 1, 0),
                    vector2d(0, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }

            //health UI
            slog("%d", player_getHealth());
            if (player_getHealth() == 1) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/health1.png", 64, 64, 1, 0),
                    vector2d(400, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (player_getHealth() == 2) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/health2.png", 64, 64, 1, 0),
                    vector2d(400, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (player_getHealth() == 3) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/health3.png", 64, 64, 1, 0),
                    vector2d(400, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (player_getHealth() == 4) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/health4.png", 64, 64, 1, 0),
                    vector2d(400, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else if (player_getHealth() == 5) {
                gf2d_sprite_draw(
                    gf2d_sprite_load_all("images/health5.png", 64, 64, 1, 0),
                    vector2d(400, 0),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
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
