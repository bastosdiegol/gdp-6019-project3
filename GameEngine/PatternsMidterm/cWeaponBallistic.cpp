#include "cWeaponBallistic.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cWeaponBallistic::cWeaponBallistic(std::string name, float damagePerShot, float damageRadius, float reloadRate) {
	DEBUG_PRINT("cWeaponBallistic::cWeaponBallistic(%s, %f, %f, %f)\n", name, damagePerShot, damageRadius, reloadRate);
	m_name = name;
	m_damagePerShot = damagePerShot;
	m_damageRadius = damageRadius;
	m_reloadRate = reloadRate;
}

cWeaponBallistic::~cWeaponBallistic() {
	DEBUG_PRINT("cWeaponBallistic::~cWeaponBallistic()\n");
}

void cWeaponBallistic::Fire() {
	DEBUG_PRINT("cWeaponBallistic::Fire()\n");
	return;
}

std::string cWeaponBallistic::getName() {
	return this->m_name;
}
