#pragma once
#include "iRobot.h"

class cRobot :
    public iRobot {
public:
    Vector3		    m_position;
    float		    m_health;

    cRobot();
    ~cRobot();

    void Update(double deltaTime);
    unsigned int getID();
    void ChangeSpawnLocation();
private:
    unsigned int    m_id;

    static const unsigned int STARTING_ID = 0;
    static const unsigned int ID_VALUE_INCREMENT = 1;
    static unsigned int m_nextID;
};

