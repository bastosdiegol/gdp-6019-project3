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
	m_cooldown		= 0.0f;
}

cWeaponLOS::~cWeaponLOS() {
	DEBUG_PRINT("cWeapon::~cWeapon()\n");
}

void cWeaponLOS::Update(float deltaTime) {
	m_cooldown -= deltaTime;
}

void cWeaponLOS::setCooldown() {
	this->m_cooldown = this->m_reloadRate;
}

std::string cWeaponLOS::getName() {
	return this->m_name;
}

float cWeaponLOS::getCooldown() {
	return m_cooldown;
}

float cWeaponLOS::getDamagePerShot() {
	return this->m_damagePerShot;
}
