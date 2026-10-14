#ifndef POLEVAULTINGZOMBIE_HPP__
#define POLEVAULTINGZOMBIE_HPP__

#include "Zombie.hpp"

enum class PVState
{
    RUNNING,
    JUMPING,
    WALKING,
    EATING
};

class PoleVaultingZombie : public Zombie
{
public:
    PoleVaultingZombie(int row, int col);

    void Update() override;

    void SetTarget(std::shared_ptr<GameObject> target) override;

private:
    PVState m_state = PVState::RUNNING;
    int m_jumpTimer = 0;
};

#endif
