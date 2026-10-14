#ifndef PEASHOOTER_HPP__
#define PEASHOOTER_HPP__

#include "Plant.hpp"

class Peashooter : public Plant
{
public:
    Peashooter(int row, int col);
    void Update() override;
    void OnClick() override {}

private:
    int m_cooldown = 0;
};

#endif