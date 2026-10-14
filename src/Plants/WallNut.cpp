#include "WallNut.hpp"

WallNut::WallNut(int row, int col)
    : Plant(ImageID::WALLNUT, row, col, 3600) {}

void WallNut::Update()
{
    if (m_hp <= 0)
    {
        SetDead();
        return;
    }

    if (m_hp < m_maxHp / 3)
    {
        ChangeImage(ImageID::WALLNUT_CRACKED);
    }
}