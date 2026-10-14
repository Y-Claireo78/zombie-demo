#include "Plant.hpp"

Plant::Plant(ImageID img, int row, int col, int hp)
    : GameObject(img,
                 LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2,
                 FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT,
                 LayerID::PLANTS,
                 60, 80,
                 AnimID::IDLE),
      m_row(row), m_col(col), m_hp(hp), m_maxHp(hp) {}

void Plant::TakeDamage(int damage)
{
    m_hp -= damage;
    if (m_hp < 0)
        m_hp = 0;
}