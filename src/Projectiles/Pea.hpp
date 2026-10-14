#ifndef PEA_HPP__
#define PEA_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Pea : public GameObject
{
public:
    Pea(int x, int y);
    void Update() override;
    void OnClick() override {}

    bool IsPea() const override { return true; }
};

#endif