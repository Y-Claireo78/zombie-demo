#ifndef ZOMBIE_HPP__
#define ZOMBIE_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Zombie : public GameObject
{
public:
    Zombie(ImageID imageID, int x, int y, int hp, int speed, AnimID animID);
    virtual ~Zombie() = default;

    bool IsZombie() const override { return true; }

    void Update() override;
    void OnClick() override {}

    virtual void TakeDamage(int damage);

    void SetTarget(std::shared_ptr<GameObject> target) override;
    bool IsEating() const { return m_target != nullptr; }

protected:
    int m_hp;
    int m_speed;
    std::shared_ptr<GameObject> m_target;
    std::shared_ptr<GameObject> GetTarget() const { return m_target; }
};

#endif