#include "cWeaponAssembler.h"
#include "cWeaponLOS.h"
#include "cWeaponBallistic.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cWeaponAssembler* cWeaponAssembler::m_weaponAssembler = nullptr;

cWeaponAssembler::cWeaponAssembler() {
	DEBUG_PRINT("cWeaponAssembler::cWeaponAssembler()\n");
}

cWeaponAssembler::~cWeaponAssembler() {
	DEBUG_PRINT("cWeaponAssembler::~cWeaponAssembler()\n");
}

cWeaponAssembler* cWeaponAssembler::GetInstance() {
	DEBUG_PRINT("cWeaponAssembler::GetInstance()\n");
	if (m_weaponAssembler == nullptr) {
		m_weaponAssembler = new cWeaponAssembler();
	}
	return m_weaponAssembler;
}

iWeapon* cWeaponAssembler::BuildAWeapon(void) {
	int randWeaponType = RandInt(1,3);
	iWeapon* theWeapon;
	switch (randWeaponType) {
	case 1 : theWeapon = new cWeaponLOS("Laser", 4.0f, 1.0f, 1.0f);
		break;
	case 2 : theWeapon = new cWeaponBallistic("Bomb", 25.0f, 5.0f, 5.0f);
		break;
	case 3 : theWeapon = new cWeaponLOS("Bullet", 2.0f, 1.0f, 0.0f);
		break;
	default:
		theWeapon = nullptr;
		break;
	}
	return theWeapon;
}

int RandInt(int min, int max) {
	if (max == min)
		return 0;

	int diff = (max - min) + 1;
	return min + (rand() % diff);
}