#ifndef REPEATER_HPP__
#define REPEATER_HPP__

#include "Plant.hpp"

class Repeater : public Plant
{
public:
    Repeater(int row, int col);
    void Update() override;
    void OnClick() override {}

private:
    int m_cooldown = 0;
    int m_secondPea = 0;
};

#endif