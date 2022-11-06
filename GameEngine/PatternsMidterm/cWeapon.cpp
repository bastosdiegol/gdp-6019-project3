#include "cWeapon.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cWeapon::cWeapon(std::string name, float damagePerShot, float damageRadius, float reloadRate) {
	m_name			= name;
	m_damagePerShot = damagePerShot;
	m_damageRadius	= damageRadius;
	m_reloadRate	= reloadRate;
}

cWeapon::~cWeapon() {
	DEBUG_PRINT("cWeapon::~cWeapon()\n");
}

void cWeapon::Fire() {
	DEBUG_PRINT("cWeapon::Fire()\n");
	return;
}
