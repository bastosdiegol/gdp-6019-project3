#pragma once
#include "Vector3.h"
#include <string>

class iRobot {
public:
	virtual ~iRobot() {};

	virtual unsigned int getID(void) = 0;
	virtual void Update(double deltaTime) = 0;
	virtual void setPosition(float x, float y, float z) = 0;
	virtual Vector3 getPosition() = 0;
	virtual float getHealth() = 0;
	virtual void setHeight(float y) = 0;
};

