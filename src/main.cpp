#include "bn_core.h"
#include <bn_sprite_ptr.h>
#include <bn_sprite_items_skull.h> // Our skull sprite

int main()
{
    bn::core::init();

    bn::sprite_ptr skull_sprite = bn::sprite_items::skull.create_sprite(0,0);

    while(true)
    {
        bn::core::update();
    }
}
