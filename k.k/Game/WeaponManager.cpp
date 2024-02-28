#include "stdafx.h"
#include "WeaponManager.h"

#include "WeaponBase.h"

#include "SwordShield.h"
#include "GreatSword.h"
#include "Bow.h"

WeaponManager* WeaponManager::m_weaponInstance = nullptr;

WeaponManager::WeaponManager()
{
	if (m_weaponInstance != nullptr) {
		//既にインスタンスがあったらエラー
		std::abort();
	}
	m_weaponInstance = this;
}


WeaponManager::~WeaponManager()
{
	m_weaponInstance = nullptr;
}

void WeaponManager::InitAllWeapon(int animClipNum)
{
	//武器の生成
	CreateAllWeapon();

	//animClipを使ってアニメーションクリップの最初の番号設定
	SetWeaponAnimationStartIndexNo(animClipNum, enMaxWeapons_Main);
	SetWeaponAnimationStartIndexNo(animClipNum, enMaxWeapons_Sub);
	SetWeaponAnimationStartIndexNo(animClipNum, enMaxWeapons_Sub2);

}

void WeaponManager::CreateAllWeapon()
{
	//ソード＆シールド
	SwordShield* swordShield = NewGO<SwordShield>(0, "swordshield");
	m_useWeapon[enMaxWeapons_Main].weapon = swordShield;
	//グレイトソード
	GreatSword* greatSword = NewGO<GreatSword>(0, "greatsword");
	m_useWeapon[enMaxWeapons_Sub].weapon = greatSword;
	//ボウ＆アロー
	Bow* bowArrow = NewGO<Bow>(0, "bowarrow");
	m_useWeapon[enMaxWeapons_Sub2].weapon = bowArrow;


}

void WeaponManager::SetWeaponAnimationStartIndexNo(int animClipNum, EnMaxWeapons setWeapon)
{
	//武器のアニメーションクリップの最初の番号を設定
	m_useWeapon[setWeapon].AnimationStartIndexNo = animClipNum * (int)setWeapon;
}

WeaponBase* WeaponManager::GetWeaponObject(EnWeaponType getWeaponType)
{
	return m_useWeapon[getWeaponType].weapon;
}

WeaponBase* WeaponManager::GetWeaponObject(EnMaxWeapons getWeapon)
{
	return m_useWeapon[getWeapon].weapon;
}

