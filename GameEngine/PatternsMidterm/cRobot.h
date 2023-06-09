#pragma once
#include <cParticle.h>
#include "iRobot.h"
#include "iWeapon.h"

class cRobot :
    public iRobot {
public:
    Vector3		    m_position;
    float		    m_health;
    iWeapon*        m_weapon;
    cParticle*      m_firedProjectile;
    iRobot*         m_curTarget;

    cRobot();
    ~cRobot();

    void            Update(float deltaTime);
    unsigned int    getID();
    // Setters
    void            setHealth(float hp);
    void            setPosition(float x, float y, float z);
    void            setHeight(float y);
    void            setWeapon(iWeapon* weapon);
    void            setProjectile(cParticle* projectile);
    void            setCurTarget(iRobot* target);
    // Getters
    Vector3         getPosition();
    float           getHealth();
    iWeapon*        getWeapon();
    std::string     getWeaponName();
    cParticle*      getFiredProjectile();
    iRobot*         getCurTarget();

private:
    unsigned int    m_id;

    static const unsigned int STARTING_ID = 0;
    static const unsigned int ID_VALUE_INCREMENT = 1;
    static unsigned int m_nextID;
};

