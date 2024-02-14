#include "stdafx.h"
#include "WeaponManager.h"
#include "SwordShield.h"
#include "GreatSword.h"
#include "Bow.h"

WeaponManager* WeaponManager::m_weaponInstance = nullptr;

WeaponManager::WeaponManager()
{
	if (m_weaponInstance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_weaponInstance = this;
}


WeaponManager::~WeaponManager()
{
	m_weaponInstance = nullptr;
}

WeaponBase* WeaponManager::CreateWeapon(EnWeaponType weaponTipe)
{
	switch (weaponTipe)
	{
	case enWeaponType_SwordShield:
		return CreateSwordShield();
		break;
	case enWeaponType_TwoHandSword:
		return CreateGreatSword();
		break;
	case enWeaponType_Bow:
		return CreateBow();
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}

void WeaponManager::SwapWeapons(
	WeaponBase* mainWeapon, WeaponBase* subWeapon)
{
	mainWeapon->ReverseWeaponState();
	subWeapon->ReverseWeaponState();

}

WeaponBase* WeaponManager::CreateSwordShield()
{
	SwordShield* swordshield = NewGO<SwordShield>(0, "swordshield");
	return swordshield;
}

WeaponBase* WeaponManager::CreateGreatSword()
{
	GreatSword* bigsword = NewGO<GreatSword>(0, "bigsword");
	return bigsword;
}

WeaponBase* WeaponManager::CreateBow()
{
	Bow* bow = NewGO<Bow>(0, "bow");
	return bow;
}

