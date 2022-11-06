#pragma once
#include "iWeapon.h"
#include <string>

class cWeapon :
    public iWeapon {
public:
    std::string m_name;
    float       m_damagePerShot;
    float       m_damageRadius;
    float       m_reloadRate;

    cWeapon(std::string name, float damagePerShot, float damageRadius, float reloadRate);
    ~cWeapon();

    void Fire();
};

