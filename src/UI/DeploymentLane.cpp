#include "DeploymentLane.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "Zombies/RegularZombie.hpp"
#include "Zombies/PoleVaultingZombie.hpp"

DeploymentLane::DeploymentLane(int row, int col)
    : GameObject(ImageID::NONE,
                 LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2,
                 FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT,
                 LayerID::UI,
                 LAWN_GRID_WIDTH, LAWN_GRID_HEIGHT - 10,
                 AnimID::NO_ANIMATION),
      m_row(row), m_col(col) {}

void DeploymentLane::OnClick()
{
    GameWorld *world = GetWorld();

    ZombieType selected = world->GetSelectedCard();
    if (selected == ZombieType::NONE)
        return;

    if (m_col < world->GetDeploymentCol())
        return;

    if (world->HasZombieAt(m_row, m_col))
        return;

    if (selected == ZombieType::REGULAR)
    {
        if (!world->SpendSun(ZOMBIE_PRICE_REGULAR))
            return;
        world->AddObject(std::make_shared<RegularZombie>(m_row, m_col));
    }
    else if (selected == ZombieType::POLEVAULTING)
    {
        if (!world->SpendSun(ZOMBIE_PRICE_POLEVAULTING))
            return;
        world->AddObject(std::make_shared<PoleVaultingZombie>(m_row, m_col));
    }
    else
    {
        return;
    }

    world->BeginCooldown(selected);
    world->ClearSelectedCard();
}
