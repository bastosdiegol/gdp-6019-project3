#pragma once
#include <string>

class iWeapon {
public:
	virtual ~iWeapon() {};

	virtual void Update(float deltaTime) = 0;
	virtual void Fire() = 0;
	virtual std::string getName() = 0;
	virtual float getCooldown() = 0;
};