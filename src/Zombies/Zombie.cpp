#include "Zombie.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "pvz/GameObject/GameObject.hpp"

Zombie::Zombie(ImageID imageID, int x, int y, int hp, int speed, AnimID animID)
    : GameObject(imageID, x, y, LayerID::ZOMBIES, 20, 80, animID),
      m_hp(hp), m_speed(speed), m_target(nullptr) {}

void Zombie::TakeDamage(int damage) {
    m_hp -= damage;
    if (m_hp < 0) m_hp = 0;
}

void Zombie::SetTarget(std::shared_ptr<GameObject> target) {
    if (target && target->IsPlant() && !target->IsDead()) {
        if (m_target != target) {
            m_target = target;
            PlayAnimation(AnimID::EAT);
        }
    } else {
        if (m_target != nullptr) {
            m_target = nullptr;
            if (m_hp > 0)
                PlayAnimation(AnimID::WALK);
        }
    }
}

void Zombie::Update() {
    if (m_hp <= 0) {
        SetDead();
        return;
    }

    if (m_target && !m_target->IsDead() && m_target->IsPlant()) {
        m_target->TakeDamage(ZOMBIE_DAMAGE);
        return;
    }

    if (m_target) {
        m_target = nullptr;
        PlayAnimation(AnimID::WALK);
    }

    MoveTo(GetX() - m_speed, GetY());
}