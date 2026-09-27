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

void Worm::update_worm(Worm::direction_map direction)
{
    // here we update all the cells. this may become quite heavy if the worm is long
}