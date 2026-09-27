#include "level.h"
#include <bn_memory.h>
#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_point.h>
#include <bn_sprite_ptr.h>
#include <bn_regular_bg_ptr.h>
#include <bn_regular_bg_items_matopeli_level.h>
#include <bn_regular_bg_map_cell_info.h>
#include <bn_vector.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_items_skull.h> // Our skull sprite
#include <bn_sprite_items_simple_worm.h>
#include <bn_sprite_items_blood.h>
#include <bn_seed_random.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include "../worm.h"

using namespace mato;

int matopeli::start_level(int seed_root = 123456)
{
    unsigned int u_seed_root = seed_root;
    bn::seed_random random(u_seed_root);

    constexpr int cell_size = 8;
    constexpr int map_width = 9;
    constexpr int map_height = 7;

    bn::regular_bg_ptr map_bg = bn::regular_bg_items::matopeli_level.create_bg(0, 0);
    // bn::sprite_ptr worm_head= bn::sprite_items::simple_worm.create_sprite(5,5,2);
    // bn::fixed_point worm_head_position = worm_head.position();
    // bn::sprite_ptr worm_tail = bn::sprite_items::simple_worm.create_sprite(worm_head_position.x(), worm_head_position.y()+8,8);

    mato::Worm worm;
    bn::vector<bn::sprite_ptr, 100> worm_sprites;

    const auto &worm_body = worm.mato_body_data();

    BN_LOG("worm body: ", worm_body.size());
    // draw the worm
    auto update_worm = [&](mato::Worm::direction_map direction) {
        worm_sprites.clear();
        worm.update_worm(direction);

        for (int i = 0; i <= worm_body.size() - 1; i++)
        {
            const auto cell = mato::cell_position(worm_body[i].x, worm_body[i].y, worm_body[i].dir);
            BN_LOG("BODY: ", cell.x, cell.y, cell.dir);

            if (i == 0)
            {
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
            if (i == worm_body.size())
            {
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
            else
            {
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
        };
    };

    while (true)
    {
        BN_LOG("START");
        if (bn::keypad::a_pressed())
        {
            update_worm(mato::Worm::direction_map::HEAD_UP);
        }
        bn::core::update();
    };
};
