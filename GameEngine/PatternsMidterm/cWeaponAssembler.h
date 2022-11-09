#pragma once
#include "iWeapon.h"

class cWeaponAssembler {
protected:
	cWeaponAssembler();

	static cWeaponAssembler* m_weaponAssembler;

public:
	~cWeaponAssembler();
	cWeaponAssembler(cWeaponAssembler& other) = delete;
	void operator=(const cWeaponAssembler&) = delete;

	/// <summary>
	/// Singleton Method for Getting an Instance of this class
	/// </summary>
	/// <returns>Pointer of WeaponAssembler</returns>
	static cWeaponAssembler* GetInstance(void);
	/// <summary>
	/// Builds a random iWeapon
	/// </summary>
	/// <returns>A pointer to the built Weapon</returns>
	iWeapon* BuildAWeapon(void);
};

