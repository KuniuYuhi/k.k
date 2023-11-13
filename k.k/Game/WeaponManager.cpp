#include "stdafx.h"
#include "WeaponManager.h"
#include "SwordShield.h"
#include "BigSword.h"
#include "Bow.h"

//todo 武器入り替え関数必要？

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
		return CreateBigSword();
		break;
	case enWeaponType_Bow:
		return CreateBow();
		break;
		/*case enWeaponType_DoubleSwords:
			return nullptr;
			break;*/
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

WeaponBase* WeaponManager::CreateBigSword()
{
	BigSword* bigsword = NewGO<BigSword>(0, "bigsword");
	return bigsword;
}

WeaponBase* WeaponManager::CreateBow()
{
	Bow* bow = NewGO<Bow>(0, "bow");
	return bow;
}

