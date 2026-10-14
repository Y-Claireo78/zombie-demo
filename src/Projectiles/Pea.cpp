#include "Pea.hpp"
#include "pvz/utils.hpp"

Pea::Pea(int x, int y)
    : GameObject(ImageID::PEA, x, y, LayerID::PROJECTILES, 28, 28, AnimID::NO_ANIMATION) {}

void Pea::Update()
{
    MoveTo(GetX() + 8, GetY());

    if (GetX() > WINDOW_WIDTH)
    {
        SetDead();
    }
}