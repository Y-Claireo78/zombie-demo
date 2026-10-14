#include "PoleVaultingZombie.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "pvz/utils.hpp"

PoleVaultingZombie::PoleVaultingZombie(int row, int col)
    : Zombie(ImageID::POLE_VAULTING_ZOMBIE,
             LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2,
             FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT,
             420, POLEVAULT_RUN_SPEED, AnimID::RUN) {}

void PoleVaultingZombie::Update()
{
    if (m_hp <= 0)
    {
        SetDead();
        return;
    }

    if (m_state == PVState::JUMPING)
    {
        --m_jumpTimer;
        if (m_jumpTimer <= 0)
        {
            m_state = PVState::WALKING;
            PlayAnimation(AnimID::WALK);
            int landX = GetX() - POLEVAULT_JUMP_DIST;
            if (landX < LAWN_GRID_LEFT)
                landX = LAWN_GRID_LEFT;
            MoveTo(landX, GetY());
        }
        return;
    }

    if (m_state == PVState::EATING)
    {
        if (m_target && !m_target->IsDead() && m_target->IsPlant())
        {
            m_target->TakeDamage(ZOMBIE_DAMAGE);
            return;
        }
        m_target = nullptr;
        m_state = PVState::WALKING;
        PlayAnimation(AnimID::WALK);
    }

    if (m_state == PVState::RUNNING)
    {
        GameWorld *world = GetWorld();
        bool plantAhead = world->HasPlantOverlap(GetX() - POLEVAULT_LOOKAHEAD_PX, GetY(), GetWidth(), GetHeight());

        if (plantAhead)
        {
            m_state = PVState::JUMPING;
            m_jumpTimer = POLEVAULT_JUMP_FRAMES;
            PlayAnimation(AnimID::JUMP);
            return;
        }
    }

    if (m_state == PVState::RUNNING)
        MoveTo(GetX() - POLEVAULT_RUN_SPEED, GetY());
    else if (m_state == PVState::WALKING)
        MoveTo(GetX() - POLEVAULT_WALK_SPEED, GetY());
}

void PoleVaultingZombie::SetTarget(std::shared_ptr<GameObject> target)
{
    if (target && target->IsPlant() && !target->IsDead())
    {
        if (m_state == PVState::WALKING && m_target != target)
        {
            m_state = PVState::EATING;
            m_target = target;
            PlayAnimation(AnimID::EAT);
        }
    }
    else
    {
        if (m_target != nullptr)
        {
            m_target = nullptr;
            if (m_hp > 0 && m_state == PVState::EATING)
            {
                m_state = PVState::WALKING;
                PlayAnimation(AnimID::WALK);
            }
        }
    }
}
