#ifndef UI_HPP__
#define UI_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Background : public GameObject
{
public:
    Background();
    void Update() override {}
    void OnClick() override {}
};

class ZombieCardBase : public GameObject
{
public:
    ZombieCardBase(ImageID img, int x, int y);
    void Update() override;
    void OnClick() override;

    void StartCooldown();
    bool IsReady() const { return m_cooldown <= 0; }

protected:
    virtual ZombieType GetCardType() const = 0;

private:
    int m_cooldown = 0;
};

class ZombieCard : public ZombieCardBase
{
public:
    ZombieCard(int x, int y);

protected:
    ZombieType GetCardType() const override { return ZombieType::REGULAR; }
};

class PoleVaultingCard : public ZombieCardBase
{
public:
    PoleVaultingCard(int x, int y);

protected:
    ZombieType GetCardType() const override { return ZombieType::POLEVAULTING; }
};

class CooldownMask : public GameObject
{
public:
    CooldownMask(int x, int y, ZombieCardBase *card);
    void Update() override;
    void OnClick() override {}

private:
    ZombieCardBase *m_card;
};

class Brain : public GameObject
{
public:
    Brain(int row);
    void Update() override {}
    void OnClick() override {}
    bool IsBrain() const override { return true; }
    int GetRow() const override { return m_row; }

private:
    int m_row;
};

class RedLine : public GameObject
{
public:
    RedLine(int x);
    void Update() override {}
    void OnClick() override {}
};

class ProgressMeter : public GameObject
{
public:
    ProgressMeter(int x, int y, ImageID img);
    void Update() override {}
    void OnClick() override {}
};

#endif