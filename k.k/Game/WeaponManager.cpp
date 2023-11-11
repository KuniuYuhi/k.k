#include "stdafx.h"
#include "WeaponManager.h"
#include "SwordShield.h"
#include "BigSword.h"

namespace MyManager {

	


}

WeaponManager* WeaponManager::m_weaponInstance = nullptr;

WeaponManager::WeaponManager()
{
	if (m_weaponInstance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_weaponInstance = this;

	AddToWeaponMap();
}


WeaponManager::~WeaponManager()
{
	m_weaponInstance = nullptr;
}

void WeaponManager::AddToWeaponMap()
{
	m_weaponMap[enWeaponType_SwordShield] = m_swordShield;
	m_weaponMap[enWeaponType_TwoHandSword] = m_bigSword;
}

