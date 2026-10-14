#ifndef DEPLOYMENTLANE_HPP__
#define DEPLOYMENTLANE_HPP__

#include "pvz/GameObject/GameObject.hpp"

class DeploymentLane : public GameObject
{
public:
    DeploymentLane(int row, int col);
    void Update() override {}
    void OnClick() override;

private:
    int m_row;
    int m_col;
};

#endif