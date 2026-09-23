#include <bn_core.h>
#include <bn_log.h>
#include <bn_timer.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_text_generator.h>
#include <bn_keypad.h>
#include <bn_alignment.h>
#include <bn_regular_bg_ptr.h>
#include <bn_regular_bg_items_matopeli_bg.h>
#include "menu.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"


int menu::init_menu()
{
    // render background
    // render "Press A to Start"
    bn::regular_bg_ptr title_bg = bn::regular_bg_items::matopeli_bg.create_bg(0,0);

    bn::sprite_text_generator title(common::variable_8x16_sprite_font);
    bn::sprite_text_generator info_text(common::variable_8x8_sprite_font);
    bn::vector<bn::sprite_ptr, 32> title_text_vector;
    bn::vector<bn::sprite_ptr, 32> info_text_vector;
    title.set_center_alignment();
    info_text.set_center_alignment();
    info_text.generate(0, 60, "Press A to start!", info_text_vector);

    // start clock
    bn::timer menu_timer;
    // wel also need a simple 2 second blinker timer for the info text. This prob needs its own reusable class.
    
    int seed_root = 0;

    while (true)
    {
        BN_LOG("S:", menu_timer.elapsed_ticks());
        // clock +1 unless <100, otherwise set to 0
        // when A pressed, get clock time and make a Randdom seed. Then
        if (menu_timer.elapsed_ticks() >= 1000000) 
        {
            menu_timer.restart();
        }
        if (bn::keypad::a_pressed())
        {
            BN_LOG("A pressed");
            seed_root = menu_timer.elapsed_ticks();
            break;
        };

        bn::core::update();
    }
    // return Random seed root
    return seed_root;
};