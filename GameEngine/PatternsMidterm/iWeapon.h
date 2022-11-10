#pragma once
#include <string>

class iWeapon {
public:
	virtual ~iWeapon() {};

	virtual void Fire() = 0;
	virtual std::string getName() = 0;
};