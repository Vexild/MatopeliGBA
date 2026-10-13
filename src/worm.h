#ifndef WORM_H
#define WORM_H
#include <bn_core.h>
#include <bn_log.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>

namespace mato
{
    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    struct cell_position
    {
        int x;
        int y;
        int dir;
    };

    class Worm
    {
    public:
        enum segment_map
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
        Worm();

        void set_direction(Direction new_direction);
        Direction get_current_direction()
        {
            return Worm::current_direction_;
        };
        bn::vector<cell_position, 100> get_body() const
        {
            return body_;
        };
        void set_body(bn::vector<cell_position, 100> new_body)
        {
            body_ = new_body;
        };
        void set_current_direction(Direction new_direction)
        {
            current_direction_ = new_direction;
        };
        void push_to_body(cell_position cell_position);
        int get_body_size()
        {
            return body_.size();
        };
        void shift_body(mato::cell_position new_head)
        {
            for (int w = get_body_size() - 1; w > 0; --w)
            {
                body_[w].x = body_[w - 1].x;
                body_[w].y = body_[w - 1].y;
                BN_LOG("body 0 x: ", body_[0].x);
                BN_LOG("body 0 y: ", body_[0].y);
            };
            body_[0] = new_head;
        };
        void reset()
        {
            body_[0].x = 4;
            body_[0].y = 4;
        }

        segment_map get_tail_sprite(cell_position tail, cell_position second_last);

    private:
        bn::vector<cell_position, 100> body_;
        Direction current_direction_;
    };
}

#endif