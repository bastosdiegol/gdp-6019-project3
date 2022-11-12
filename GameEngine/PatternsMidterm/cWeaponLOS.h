#pragma once
#include "iWeapon.h"
#include <string>

class cWeaponLOS :
    public iWeapon {
public:
    std::string m_name;
    float       m_damagePerShot;
    float       m_damageRadius;
    float       m_reloadRate;
    float       m_cooldown;

    cWeaponLOS(std::string name, float damagePerShot, float damageRadius, float reloadRate);
    ~cWeaponLOS();

    void Update(float deltaTime);
    void setCooldown();
    std::string getName();
    float getCooldown();
    float getDamagePerShot();
};

