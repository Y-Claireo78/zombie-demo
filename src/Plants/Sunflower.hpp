#ifndef SUNFLOWER_HPP__
#define SUNFLOWER_HPP__

#include "Plant.hpp"

class Sunflower : public Plant
{
public:
    Sunflower(int row, int col);
    void Update() override;
    void OnClick() override {}

    void OnEaten();

private:
    bool m_hasDropped = false;
};

#endif