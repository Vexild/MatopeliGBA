#include <array>
#include <string>
#include <bn_vector.h>
#include "level.h"
#include <bn_memory.h>
#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_timer.h>
#include <bn_timers.h>
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

struct Cell
{
    int x;
    int y;
};

int matopeli::start_level(int seed_root = 123456)
{
    unsigned int u_seed_root = seed_root;
    bn::seed_random random(u_seed_root);

    constexpr int CELL_SIZE = 8;
    constexpr int MAP_WIDTH = 9;
    constexpr int MAP_HEIGHT = 7;

    // Hoping this may come handy later
    using Map = std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>;
    Map map = {};

    bn::regular_bg_ptr map_bg = bn::regular_bg_items::matopeli_level.create_bg(0, 0);

    mato::Worm worm;
    
    bn::vector<bn::sprite_ptr, 100> worm_sprites;
    bn::timer timer;
    uint64_t ticks = 0;
    uint64_t speed = 1.00;
    uint64_t frame_limit;
    mato::Direction current_direction = mato::Direction::UP;
    bool turbo = false;
    bool direction_input_lock = false;

    // draw the worm. TODO: So far we only render the head. Code wont render rest of the worm.
    auto update_worm = [&]()
    {
        worm_sprites.clear();
        for (int i = 0; i <= worm.get_body_size() - 1; i++)
        {
            bn::vector<mato::cell_position, 100> worm_body = worm.get_body();
            auto cell = mato::cell_position(worm_body[i].x, worm_body[i].y, worm_body[i].dir);
            if (i == 0)
            {
                // BN_LOG("Head on: ",cell.x, cell.y);
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
            else if (i == worm.get_body_size() - 1)
            {
                //BN_LOG(i, "TAIL PART : ", cell.x, cell.y);
                // TODO: Get the proper orientation with worm.cpp. We dont want to include the logic here, only draw the sprites.
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
                // worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, worm.get_tail_sprite(worm.get_body()[i], worm.get_body()[i-1])));
            }
            else
            {
                // BN_LOG("Tail on: ", cell.x, cell.y);
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
        };
    };

    auto opposite_dir = [&](mato::Direction dir)
    {
        //BN_LOG("Opposite? ", (int)dir != (int)current_direction);
        return (int)dir != (int)current_direction;
    };

    while (true)
    {
        mato::cell_position worm_head = worm.get_body()[0];

        ticks += timer.elapsed_ticks_with_restart();
        uint64_t frames = ticks / bn::timers::ticks_per_frame();
        frame_limit = turbo ? 10 : 20;
        if (frames >= frame_limit)
        {
            switch (worm.get_current_direction())
            {
            case mato::Direction::UP:
                worm_head.y -= speed;
                break;
            case mato::Direction::DOWN:
                worm_head.y += speed;
                break;
            case mato::Direction::LEFT:
                worm_head.x -= speed;
                break;
            case mato::Direction::RIGHT:
                worm_head.x += speed;
                break;
            default:
                break;
            };
            // shift all worm segments
            worm.shift_body(worm_head);
            
            direction_input_lock = false;
            ticks = 0;
        };

        // Controls need a input lock: Time when we cannot take in inputs
        if (bn::keypad::up_pressed() && opposite_dir(mato::Direction::DOWN) && !direction_input_lock)
        {
            current_direction = mato::Direction::UP;
            worm.set_direction(mato::Direction::UP);
            direction_input_lock = true;
        };
        if (bn::keypad::down_pressed() && opposite_dir(mato::Direction::UP) && !direction_input_lock)
        {
            current_direction = mato::Direction::DOWN;
            worm.set_direction(mato::Direction::DOWN);
            direction_input_lock = true;
        };
        if (bn::keypad::left_pressed() && opposite_dir(mato::Direction::RIGHT) && !direction_input_lock)
        {
            current_direction = mato::Direction::LEFT;
            worm.set_direction(mato::Direction::LEFT);
            direction_input_lock = true;
        };
        if (bn::keypad::right_pressed() && opposite_dir(mato::Direction::LEFT) && !direction_input_lock)
        {
            current_direction = mato::Direction::RIGHT;
            worm.set_direction(mato::Direction::RIGHT);
            direction_input_lock = true;
        };
        if (bn::keypad::b_held())
        {
            turbo = true;
        };
        if (bn::keypad::b_released())
        {
            turbo = false;
        };
        if (bn::keypad::start_pressed())
        {
            worm.reset();
        };
        // const Cell &head = worm.body.front();
        //  BN_LOG("head: ", head.x, head.y, "size: ", worm.body.size(), "dir: ", (int)worm.dir);
        update_worm();
        bn::core::update();
    };
};
