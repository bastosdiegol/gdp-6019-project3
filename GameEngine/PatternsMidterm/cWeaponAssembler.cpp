#include "cWeaponAssembler.h"
#include "cWeapon.h"

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