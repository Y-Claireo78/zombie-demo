#include "Repeater.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "Projectiles/Pea.hpp"

Repeater::Repeater(int row, int col)
    : Plant(ImageID::REPEATER, row, col, 340) {}

void Repeater::Update()
{
    if (m_hp <= 0)
    {
        SetDead();
        return;
    }

    if (m_secondPea > 0)
    {
        --m_secondPea;
        if (m_secondPea == 0)
        {
            GameWorld *world = GetWorld();
            if (world)
            {
                auto pea = std::make_shared<Pea>(GetX() + 30, GetY() + 20);
                world->AddObject(pea);
            }
        }
        return;
    }

    if (m_cooldown > 0)
    {
        --m_cooldown;
        return;
    }

    GameWorld *world = GetWorld();
    if (world->HasZombieInRow(m_row, GetX()))
    {
        auto pea1 = std::make_shared<Pea>(GetX() + 30, GetY() + 20);
        world->AddObject(pea1);

        m_cooldown = PEASHOOTER_COOLDOWN;
        m_secondPea = 8;
    }
}