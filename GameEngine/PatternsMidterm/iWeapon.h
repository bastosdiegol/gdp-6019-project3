#pragma once

class iWeapon {
public:
	virtual ~iWeapon() {};

	virtual void Fire() = 0;
};