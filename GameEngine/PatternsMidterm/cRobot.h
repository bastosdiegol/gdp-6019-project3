#pragma once
#include "iRobot.h"
#include "iWeapon.h"

class cRobot :
    public iRobot {
public:
    Vector3		    m_position;
    float		    m_health;
    iWeapon*        m_weapon;

    cRobot();
    ~cRobot();

    void            Update(double deltaTime);
    unsigned int    getID();
    void            setPosition(float x, float y, float z);
    Vector3         getPosition();
    float           getHealth();
    void            setHeight(float y);
    void            setWeapon(iWeapon* weapon);
    std::string     getWeaponName();
    void            ChangeSpawnLocation();
private:
    unsigned int    m_id;

    static const unsigned int STARTING_ID = 0;
    static const unsigned int ID_VALUE_INCREMENT = 1;
    static unsigned int m_nextID;
};

