#include "bn_core.h"
#include <bn_sprite_ptr.h>
#include <bn_sprite_items_skull.h> // Our skull sprite
#include <bn_sprite_items_blood.h>
#include <bn_regular_bg_ptr.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_vector.h>
#include <bn_seed_random.h>
#include "levels/level.h"
#include "levels/menu.h"

int main()
{
    bn::core::init();

    int random_seed = menu::init_menu();
    BN_LOG("SEED: ", random_seed);
    matopeli::start_level(random_seed);
    
    while (true)
    {
        bn::core::update();
    }
};
