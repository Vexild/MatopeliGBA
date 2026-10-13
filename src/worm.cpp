#include "worm.h"
using namespace mato;

Worm::Worm()
{
    body_.push_back({4, 3, segment_map::HEAD_UP});
    body_.push_back({3, 3, segment_map::BODY_VERTICAL});
    body_.push_back({2, 3, segment_map::BODY_LEFT_UP});
    body_.push_back({2, 2, segment_map::BODY_HORIZONTAL});
    body_.push_back({2, 1, segment_map::TAIL_LEFT});
    current_direction_ = Direction::UP;
}

void Worm::set_direction(Direction new_direction)
{
    current_direction_ = new_direction;
};

void Worm::push_to_body(cell_position cell_position)
{
    body_.push_back(cell_position);
};

mato::Worm::segment_map get_tail_sprite(cell_position tail, cell_position second_last)
{
    if (tail.x > second_last.x)
    {
        return mato::Worm::TAIL_LEFT;
    }
    else
    {
        return mato::Worm::TAIL_RIGHT;
    }

    if (tail.y > second_last.y)
    {
        return mato::Worm::TAIL_UP;
    }
    else
    {
        return mato::Worm::TAIL_DOWN;
    }
};