#include "Sunflower.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "UI/Sun.hpp"
#include "pvz/utils.hpp"
Sunflower::Sunflower(int row, int col)
    : Plant(ImageID::SUNFLOWER, row, col, 340) {}

void Sunflower::Update()
{
    if (m_hp <= 0)
    {
        if (!m_hasDropped)
        {
            OnEaten();
            m_hasDropped = true;
        }
        SetDead();
        return;
    }
}

void Sunflower::OnEaten()
{
    GameWorld *world = GetWorld();

    for (int i = 0; i < 6; ++i)
    {
        int offsetX = randInt(-30, 30);
        int offsetY = randInt(-20, 20);
        auto sun = std::make_shared<Sun>(GetX() + offsetX, GetY() + offsetY);
        world->AddObject(sun);
    }
}