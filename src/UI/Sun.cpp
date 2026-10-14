#include "Sun.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "pvz/utils.hpp"

Sun::Sun(int x, int y)
    : GameObject(ImageID::SUN, x, y, LayerID::SUN, 40, 40, AnimID::IDLE) {}

void Sun::Update()
{
}

void Sun::OnClick()
{
    GameWorld *world = GetWorld();
    world->AddSun(SUN_VALUE);
    SetDead();
}