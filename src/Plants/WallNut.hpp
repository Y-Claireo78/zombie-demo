#ifndef WALLNUT_HPP__
#define WALLNUT_HPP__

#include "Plant.hpp"

class WallNut : public Plant
{
public:
    WallNut(int row, int col);
    void Update() override;
    void OnClick() override {}
};

#endif