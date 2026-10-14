#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include "pvz/Framework/ObjectBase.hpp"

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;

class GameObject : public ObjectBase
{
public:
    GameObject(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID)
        : ObjectBase(imageID, x, y, layer, width, height, animID) {}
    virtual ~GameObject() = default;
    virtual void Update() = 0;
    virtual void OnClick() = 0;

    virtual bool IsPlant() const { return false; }
    virtual bool IsZombie() const { return false; }
    virtual bool IsPea() const { return false; }
    virtual bool IsSun() const { return false; }
    virtual bool IsBrain() const { return false; }
    virtual void TakeDamage(int damage) { (void)damage; }

    virtual int GetRow() const { return -1; }
    virtual int GetCol() const { return -1; }
    virtual void SetTarget(std::shared_ptr<GameObject> target) { (void)target; }

    void SetWorld(GameWorld *world) { m_world = world; }
    GameWorld *GetWorld() const { return m_world; }

    bool IsDead() const { return m_isDead; }
    void SetDead() { m_isDead = true; }

private:
    GameWorld *m_world = nullptr;
    bool m_isDead = false;
};

#endif
