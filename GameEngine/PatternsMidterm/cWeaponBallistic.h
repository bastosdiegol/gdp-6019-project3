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
    float       m_cooldown;

    cWeaponBallistic(std::string name, float damagePerShot, float damageRadius, float reloadRate);
    ~cWeaponBallistic();

    void Update(float deltaTime);
    void Fire();
    void setCooldown();
    std::string getName();
    float getCooldown();
};

