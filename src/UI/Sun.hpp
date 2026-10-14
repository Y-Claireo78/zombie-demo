#ifndef SUN_HPP__
#define SUN_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Sun : public GameObject
{
public:
    Sun(int x, int y);
    void Update() override;
    void OnClick() override;

    bool IsSun() const override { return true; }
};

#endif