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

    cRobot();
    ~cRobot();

    void            Update(float deltaTime);
    unsigned int    getID();
    // Setters
    void            setPosition(float x, float y, float z);
    void            setHeight(float y);
    void            setWeapon(iWeapon* weapon);
    void            setProjectile(cParticle* projectile);
    // Getters
    Vector3         getPosition();
    float           getHealth();
    iWeapon*        getWeapon();
    std::string     getWeaponName();
    cParticle*      getFiredProjectile();
private:
    unsigned int    m_id;

    static const unsigned int STARTING_ID = 0;
    static const unsigned int ID_VALUE_INCREMENT = 1;
    static unsigned int m_nextID;
};

