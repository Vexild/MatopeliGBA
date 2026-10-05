#include <array>
#include <string>
#include <vector>
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

int matopeli::start_level(int seed_root = 123456)
{
    unsigned int u_seed_root = seed_root;
    bn::seed_random random(u_seed_root);

    constexpr int CELL_SIZE = 8;
    constexpr int MAP_WIDTH = 9;
    constexpr int MAP_HEIGHT = 7;

    struct Cell
    {
        int x;
        int y;
    };

    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    // Hoping this may come handy later
    using Map = std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>;
    Map map = {};

    bn::regular_bg_ptr map_bg = bn::regular_bg_items::matopeli_level.create_bg(0, 0);

    struct Worm
    {
        std::vector<Cell> body;
        Direction dir;
    };

    Worm worm;
    worm.body = {
        {4, 3},
        {3, 3},
        {2, 3},
    };

    bn::vector<bn::sprite_ptr, 100> worm_sprites;
    
    // draw the worm. TODO: So far we only render the head. Code wont render rest of the worm.
    auto update_worm = [&]()
    {
        worm_sprites.clear();
        for (int i = 0; i <= worm.body.size() - 1; i++)
        {
            const auto cell = mato::cell_position(worm.body[i].x, worm.body[i].y);
            if (i == 0)
            {
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
            if (i == worm.body.size())
            {
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
            else
            {
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
        };
    };

    bn::timer timer;
    uint64_t ticks = 0;
    uint64_t speed = 1.00;
    uint64_t frame_limit = 20;
    bool turbo = false;

    while (true)
    {
        Cell next_head = worm.body.front();

        ticks += timer.elapsed_ticks_with_restart();
        int frames = ticks / bn::timers::ticks_per_frame();
        BN_LOG("frames: ",frames, ", SPEED: ", int(speed));
        if (turbo) {
            frame_limit = 10;
        } else {
            frame_limit = 20;
        }
        if (frames >= frame_limit) {
            BN_LOG("MOVING WORM");
            switch (worm.dir)
            {
                case Direction::UP:
                next_head.y -= speed;
                break;
                case Direction::DOWN:
                next_head.y += speed;
                break;
                case Direction::LEFT:
                next_head.x -= speed;
                break;
                case Direction::RIGHT:
                next_head.x += speed;
                break;
                default:
                break;
            };
           ticks = 0;
        }

        worm.body.front() = next_head;
        update_worm();
        
        // Controls need a polar-limiter: no turning 180 degrees.
        if (bn::keypad::up_pressed())
        {
            worm.dir = Direction::UP;
        };
        if (bn::keypad::down_pressed())
        {
            worm.dir = Direction::DOWN;
        };
        if (bn::keypad::left_pressed())
        {
            worm.dir = Direction::LEFT;
        };
        if (bn::keypad::right_pressed())
        {
            worm.dir = Direction::RIGHT;
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
            worm.body[0] = {4, 4};
        };
        const Cell &head = worm.body.front();
        // BN_LOG("head: ", head.x, head.y, "size: ", worm.body.size(), "dir: ", (int)worm.dir);
        bn::core::update();
    };
};
