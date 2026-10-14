#ifndef PLANT_HPP__
#define PLANT_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Plant : public GameObject
{
public:
    Plant(ImageID img, int row, int col, int hp);

    bool IsPlant() const override { return true; }

    virtual void TakeDamage(int damage) override;

    int GetRow() const override { return m_row; }
    int GetCol() const override { return m_col; }

protected:
    int m_row;
    int m_col;
    int m_hp;
    int m_maxHp;
};

#endif