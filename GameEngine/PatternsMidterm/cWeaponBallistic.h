#pragma once
#include "iWeapon.h"
#include <string>

class cWeaponBallistic :
    public iWeapon {
public:
    std::string m_name;
    float       m_damagePerShot;
    float       m_damageRadius;
    float       m_reloadRate;

    cWeaponBallistic(std::string name, float damagePerShot, float damageRadius, float reloadRate);
    ~cWeaponBallistic();

    void Fire();
    std::string getName();
};

