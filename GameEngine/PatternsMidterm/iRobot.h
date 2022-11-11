#pragma once
#include "Vector3.h"
#include "iWeapon.h"
#include <string>
#include <cParticle.h>

class iRobot {
public:
	virtual ~iRobot() {};

	virtual unsigned int getID(void) = 0;
	virtual void Update(double deltaTime) = 0;
	virtual void setPosition(float x, float y, float z) = 0;
	virtual Vector3 getPosition() = 0;
	virtual float getHealth() = 0;
	virtual void setHeight(float y) = 0;
	virtual void setWeapon(iWeapon* weapon) = 0;
	virtual std::string getWeaponName() = 0;
	virtual void setProjectile(cParticle* projectile) = 0;
	virtual cParticle* getFiredProjectile() = 0;
};

