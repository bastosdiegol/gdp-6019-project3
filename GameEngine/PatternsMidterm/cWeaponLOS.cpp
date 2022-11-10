#include "cWeaponLOS.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cWeaponLOS::cWeaponLOS(std::string name, float damagePerShot, float damageRadius, float reloadRate) {
	DEBUG_PRINT("cWeaponLOS::cWeaponLOS(%s, %f, %f, %f)\n", name, damagePerShot, damageRadius, reloadRate);
	m_name			= name;
	m_damagePerShot = damagePerShot;
	m_damageRadius	= damageRadius;
	m_reloadRate	= reloadRate;
}

cWeaponLOS::~cWeaponLOS() {
	DEBUG_PRINT("cWeapon::~cWeapon()\n");
}

void cWeaponLOS::Fire() {
	DEBUG_PRINT("cWeaponLOS::Fire()\n");
	return;
}

std::string cWeaponLOS::getName() {
	return this->m_name;
}
