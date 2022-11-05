#pragma once
#include "iRobot.h"

class cRobot :
    public iRobot {
public: 
    cRobot();

    void Update(double deltaTime);
    void ChangeSpawnLocation();
private:

};

