#include "worm.h"
using namespace mato;


void Worm::set_direction(Worm::direction_map new_direction)
{
    Worm::direction_ = new_direction;
};


void Worm::push_to_body(cell_position cell_position)
{
    mato_vector_.push_back(cell_position);
};

