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

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

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
        {2, 2},
        {2, 1},
    };
    worm.dir = Direction::UP;

    bn::vector<bn::sprite_ptr, 100> worm_sprites;
    bn::timer timer;
    uint64_t ticks = 0;
    uint64_t speed = 1.00;
    uint64_t frame_limit;
    Direction current_direction = Direction::UP;
    bool turbo = false;
    bool direction_input_lock = false;

    // draw the worm. TODO: So far we only render the head. Code wont render rest of the worm.
    auto update_worm = [&]()
    {
        worm_sprites.clear();
        for (int i = 0; i <= worm.body.size() - 1; i++)
        {
            const auto cell = mato::cell_position(worm.body[i].x, worm.body[i].y);
            if (i == 0)
            {
                // BN_LOG("Head on: ",cell.x, cell.y);
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
            if (i < worm.body.size())
            {
                // BN_LOG(i, " part on: ", cell.x, cell.y);
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
            else
            {
                // BN_LOG("Tail on: ", cell.x, cell.y);
                worm_sprites.push_back(bn::sprite_items::simple_worm.create_sprite(cell.x * 8, cell.y * 8, cell.dir));
            }
        };
    };

    auto shift_body = [&]()
    {
        for (int w = worm.body.size() - 1; w > 0; w--)
        {
            worm.body[w].x = worm.body[w - 1].x;
            worm.body[w].y = worm.body[w - 1].y;
        }
    };
    auto opposite_dir = [&](Direction dir)
    {
        BN_LOG("Opposite? ", (int)dir != (int)current_direction);
        return (int)dir != (int)current_direction;
    };

    while (true)
    {
        Cell worm_head = worm.body.front();

        ticks += timer.elapsed_ticks_with_restart();
        uint64_t frames = ticks / bn::timers::ticks_per_frame();
        frame_limit = turbo ? 10 : 20;
        if (frames >= frame_limit)
        {
            switch (worm.dir)
            {
            case Direction::UP:
                worm_head.y -= speed;
                break;
            case Direction::DOWN:
                worm_head.y += speed;
                break;
            case Direction::LEFT:
                worm_head.x -= speed;
                break;
            case Direction::RIGHT:
                worm_head.x += speed;
                break;
            default:
                break;
            };
            shift_body();
            worm.body.front() = worm_head;
            direction_input_lock = false;
            ticks = 0;
        };

        // Controls need a input lock: Time when we cannot take in inputs
        if (bn::keypad::up_pressed() && opposite_dir(Direction::DOWN) && !direction_input_lock)
        {
            current_direction = Direction::UP;
            worm.dir = Direction::UP;
            direction_input_lock = true;
        };
        if (bn::keypad::down_pressed() && opposite_dir(Direction::UP) && !direction_input_lock)
        {
            current_direction = Direction::DOWN;
            worm.dir = Direction::DOWN;
            direction_input_lock = true;
        };
        if (bn::keypad::left_pressed() && opposite_dir(Direction::RIGHT) && !direction_input_lock)
        {
            current_direction = Direction::LEFT;
            worm.dir = Direction::LEFT;
            direction_input_lock = true;
        };
        if (bn::keypad::right_pressed() && opposite_dir(Direction::LEFT) && !direction_input_lock)
        {
            current_direction = Direction::RIGHT;
            worm.dir = Direction::RIGHT;
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
            worm.body[0] = {4, 4};
        };
        // const Cell &head = worm.body.front();
        //  BN_LOG("head: ", head.x, head.y, "size: ", worm.body.size(), "dir: ", (int)worm.dir);
        update_worm();
        bn::core::update();
    };
};
