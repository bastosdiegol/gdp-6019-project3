#pragma once
#include <string>

class iWeapon {
public:
	virtual ~iWeapon() {};

	virtual void Update(float deltaTime) = 0;
	virtual void setCooldown() = 0;
	virtual std::string getName() = 0;
	virtual float getCooldown() = 0;
	virtual float getDamagePerShot() = 0;
};