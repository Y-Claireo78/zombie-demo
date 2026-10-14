#include "Peashooter.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "Projectiles/Pea.hpp"

Peashooter::Peashooter(int row, int col)
    : Plant(ImageID::PEASHOOTER, row, col, 340) {}

void Peashooter::Update()
{
    if (m_hp <= 0)
    {
        SetDead();
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
        auto pea = std::make_shared<Pea>(GetX() + 30, GetY() + 20);
        world->AddObject(pea);
        m_cooldown = PEASHOOTER_COOLDOWN;
    }
}