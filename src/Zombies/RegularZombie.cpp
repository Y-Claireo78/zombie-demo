#include "RegularZombie.hpp"
#include "pvz/utils.hpp"

RegularZombie::RegularZombie(int row, int col)
    : Zombie(ImageID::REGULAR_ZOMBIE,
             LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2,
             FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT,
             260, 1, AnimID::WALK) {}