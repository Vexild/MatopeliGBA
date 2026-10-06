#include "test_level.h"
#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_items_skull.h> // Our skull sprite
#include <bn_sprite_items_blood.h>
#include <bn_seed_random.h>
#include <bn_keypad.h>
#include <bn_log.h>


int testmatopeli::start_test_level(int test_seed_root = 123456)
{
    unsigned int u_seed_root = test_seed_root;
    bn::seed_random random(u_seed_root);
    bn::sprite_ptr skull_sprite = bn::sprite_items::skull.create_sprite(0, 0);

    bn::vector<bn::sprite_ptr, 5> blood_sprites_vector;

    while (true)
    {   

        if (bn::keypad::a_pressed())
        {
            if (blood_sprites_vector.size() == blood_sprites_vector.max_size())
            {
                blood_sprites_vector.erase(blood_sprites_vector.begin());
            }
            int x = random.get_int(-120, 120);
            int y = random.get_int(-80, 80);
            blood_sprites_vector.push_back(bn::sprite_items::blood.create_sprite(x, y));
        }

        if (bn::keypad::left_held())
        {
            BN_LOG("Left held");
            bn::fixed newX = skull_sprite.x();
            newX -= 2;
            skull_sprite.set_x(newX);
        }
        if (bn::keypad::right_held())
        {
            BN_LOG("Right held");
            bn::fixed newX = skull_sprite.x();
            newX += 2;
            skull_sprite.set_x(newX);
        }

        if (bn::keypad::up_held())
        {
            BN_LOG("Up held");
            bn::fixed newY = skull_sprite.y();
            newY -= 2;
            skull_sprite.set_y(newY);
        }
        if (bn::keypad::down_held())
        {
            BN_LOG("Down held");
            bn::fixed newY = skull_sprite.y();
            newY += 2;
            skull_sprite.set_y(newY);
        }

        if (bn::keypad::start_held())
        {
            bn::fixed horizontalScale = skull_sprite.horizontal_scale();
            bn::fixed verticalScale = skull_sprite.vertical_scale();

            horizontalScale += bn::fixed(0.02);
            verticalScale += bn::fixed(0.02);
            skull_sprite.set_horizontal_scale(horizontalScale);
            skull_sprite.set_vertical_scale(verticalScale);
        }
        if (bn::keypad::select_held())
        {
            bn::fixed horizontalScale = skull_sprite.horizontal_scale();
            bn::fixed verticalScale = skull_sprite.vertical_scale();

            horizontalScale -= bn::fixed(0.02);
            verticalScale -= bn::fixed(0.02);
            skull_sprite.set_horizontal_scale(horizontalScale);
            skull_sprite.set_vertical_scale(verticalScale);
        }

        bn::fixed rotation = skull_sprite.rotation_angle();
        rotation += bn::fixed(0.5);
        skull_sprite.set_rotation_angle_safe(rotation);
        // set_rotaion_angle() function also exist but it requires values between 0 and 360

        // Coordinates
        //BN_LOG("Sprite location: ", skull_sprite.x(), skull_sprite.y());
        
        bn::core::update();
    }
}
