#include <bn_core.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_text_generator.h>
#include <bn_keypad.h>
#include <bn_alignment.h>
#include "menu.h"
#include "common_fixed_8x8_sprite_font.h"

int menu::init_menu()
{
    // render background
    // render "Press A to Start"
    bn::sprite_text_generator info_text(common::fixed_8x8_sprite_font);
    bn::vector<bn::sprite_ptr, 32> text;
    info_text.generate(-48, 0, "Press A to start!", text);

    // start clock
    while (true)
    {
        // clock +1 unless <100, otherwise set to 0
        // when A pressed, get clock time and make a Randdom seed. Then

        if (bn::keypad::a_pressed())
        {
            BN_LOG("A pressed");
            break;
        };

        bn::core::update();
    }
    // return Random seed
    return 100;
};