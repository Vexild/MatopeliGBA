#ifndef WORM_H
#define WORM_H
#include <bn_core.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>

namespace mato
{
    struct cell_position
    {
        int x;
        int y;
        int dir;
    };

    class Worm
    {
    public:
        int size = 3;
        enum direction_map
        {
            HEAD_UP = 2,
            HEAD_LEFT = 1,
            HEAD_DOWN = 3,
            HEAD_RIGHT = 0,
            BODY_RIGHT_DOWN = 4,
            BODY_LEFT_DOWN = 5,
            BODY_UP_RIGHT = 6,
            BODY_LEFT_UP = 7,
            BODY_VERTICAL = 8,
            BODY_HORIZONTAL = 9,
            TAIL_UP = 10,
            TAIL_RIGHT = 11,
            TAIL_DOWN = 12,
            TAIL_LEFT = 13
        };
        Worm()
        {
            push_to_body(cell_position{5, 5, Worm::direction_map::HEAD_UP});
            push_to_body(cell_position{4, 5, Worm::direction_map::BODY_VERTICAL});
            push_to_body(cell_position{3, 5, Worm::direction_map::TAIL_UP});
        }

        void set_direction(direction_map new_direction);
        Worm::direction_map get_direction()
        {
            return Worm::direction_;
        };
        const bn::vector<cell_position, 100> &mato_body_data() const
        {
            return mato_vector_;
        };
        void push_to_body(cell_position cell_position);
        int get_size()
        {
            return size;
        };
        void increase_size()
        {
            size += 1;
        }
        void update_worm(direction_map direction);

    private:
        bn::vector<cell_position, 100> mato_vector_;
        direction_map direction_ = direction_map::HEAD_UP;
    };
}

#endif