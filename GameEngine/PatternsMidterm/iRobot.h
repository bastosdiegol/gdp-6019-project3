#pragma once
#include "Vector3.h"
#include <string>

class iRobot {
public:
	virtual ~iRobot() {};

	virtual unsigned int getID(void) = 0;
	virtual void Update(double deltaTime) = 0;
};

