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
	m_name			= name;
	m_damagePerShot = damagePerShot;
	m_damageRadius	= damageRadius;
	m_reloadRate	= reloadRate;
	m_cooldown		= 0.0f;
}

cWeaponBallistic::~cWeaponBallistic() {
	DEBUG_PRINT("cWeaponBallistic::~cWeaponBallistic()\n");
}

void cWeaponBallistic::Update(float deltaTime) {
	this->m_cooldown -= deltaTime;
}

void cWeaponBallistic::setCooldown() {
	this->m_cooldown = m_reloadRate;
}

std::string cWeaponBallistic::getName() {
	return this->m_name;
}

float cWeaponBallistic::getCooldown() {
	return m_cooldown;
}

float cWeaponBallistic::getDamagePerShot() {
	return this->m_damagePerShot;
}
