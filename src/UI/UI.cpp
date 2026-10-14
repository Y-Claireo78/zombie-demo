#include "pvz/GameWorld/GameWorld.hpp"
#include "UI.hpp"
#include "pvz/utils.hpp"

Background::Background()
    : GameObject(ImageID::BACKGROUND, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
                 LayerID::BACKGROUND, WINDOW_WIDTH, WINDOW_HEIGHT,
                 AnimID::NO_ANIMATION) {}

ZombieCardBase::ZombieCardBase(ImageID img, int x, int y)
    : GameObject(img, x, y, LayerID::UI, SEED_WIDTH, SEED_HEIGHT, AnimID::NO_ANIMATION) {}

void ZombieCardBase::Update()
{
    if (m_cooldown > 0)
        --m_cooldown;
}

void ZombieCardBase::OnClick()
{
    GameWorld *world = GetWorld();
    if (!world)
        return;

    if (m_cooldown > 0)
        return;

    if (world->GetSelectedCard() == GetCardType())
    {
        world->ClearSelectedCard();
        return;
    }

    world->SetSelectedCard(GetCardType());
}

void ZombieCardBase::StartCooldown()
{
    m_cooldown = ZOMBIE_CARD_COOLDOWN_TICKS;
}

ZombieCard::ZombieCard(int x, int y)
    : ZombieCardBase(ImageID::ZOMBIE_CARD_REGULAR, x, y) {}

PoleVaultingCard::PoleVaultingCard(int x, int y)
    : ZombieCardBase(ImageID::ZOMBIE_CARD_POLE, x, y) {}

CooldownMask::CooldownMask(int x, int y, ZombieCardBase *card)
    : GameObject(ImageID::NONE, x, y, LayerID::COOLDOWN_MASK,
                 0, 0, AnimID::NO_ANIMATION),
      m_card(card) {}

void CooldownMask::Update()
{
    if (!m_card)
        return;
    if (m_card->IsReady())
        ChangeImage(ImageID::NONE);
    else
        ChangeImage(ImageID::COOLDOWN_MASK);
}

Brain::Brain(int row)
    : GameObject(ImageID::BRAIN_ICON,
                 35, FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT,
                 LayerID::UI, 32, 31, AnimID::NO_ANIMATION),
      m_row(row) {}

RedLine::RedLine(int x)
    : GameObject(ImageID::RED_LINE, x, LAWN_GRID_CENTER_Y,
                 LayerID::UI, 10, LAWN_GRID_TOP - LAWN_GRID_BOTTOM,
                 AnimID::NO_ANIMATION) {}

ProgressMeter::ProgressMeter(int x, int y, ImageID img)
    : GameObject(img, x, y, LayerID::UI, 100, 20, AnimID::NO_ANIMATION) {}