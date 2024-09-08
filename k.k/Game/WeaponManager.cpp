#include "stdafx.h"
#include "WeaponManager.h"

#include "SwordShield.h"
#include "GreateSword.h"
#include "Bow.h"



WeaponManager* WeaponManager::m_instance = nullptr;

WeaponManager::WeaponManager()
{
	if (m_instance != nullptr)
	{
		std::abort();
	}
	m_instance = this;
}

WeaponManager::~WeaponManager()
{
	m_instance = nullptr;
}

void WeaponManager::CreateInstance()
{
	m_instance = new WeaponManager();
}

void WeaponManager::Init(
	EnWeaponType main, EnWeaponType sub, EnWeaponType sub2)
{
	//武器をマップに格納する
	m_weaponsMap.insert(std::make_pair(enSwordShield, NewGO<SwordShield>(0, "SwordShield")));
	m_weaponsMap.insert(std::make_pair(enGreateSword, NewGO<GreateSword>(0, "GreateSword")));
	m_weaponsMap.insert(std::make_pair(enBowArrow, NewGO<Bow>(0, "Bow")));

	//全ての武器の状態を収納状態にする
	for (auto& weapon : m_weaponsMap)
	{
		weapon.second->ChangeStowedState();
	}
	
	//武器の役割を設定する
	SettingUseWeaponRole(main, sub, sub2);

	//メイン武器のキーに対応する要素を取得。現在の武器とする
	m_armedWeapon = m_weaponsMap.at(m_mainWeaponType);
	//武器を装備状態にする
	m_armedWeapon->SetCurrentWeaponState(WeaponBase::enArmed);

}

void WeaponManager::SettingUseWeaponRole(
	EnWeaponType main, EnWeaponType sub, EnWeaponType sub2)
{
	m_mainWeaponType = main;
	m_subWeaponType = sub;
	m_subWeaponType2 = sub2;
}

void WeaponManager::ChangeArmedWeapon(EnWeaponType armedWeaponType)
{
	//現在装備している武器を収納状態にする
	m_armedWeapon->ChangeStowedState();

	//装備する武器をマップから持ってくる
	m_armedWeapon = m_weaponsMap.at(armedWeaponType);
	//マップになければ
	if (m_armedWeapon == nullptr)
	{
		std::abort();
	}
	//武器を装備状態にする
	m_armedWeapon->ChangeArmedState();

	//メイン武器の中身を切り替え対象武器に切り替える
	ChangeChangeWeaponTypeToMainWeaponType();
}

void WeaponManager::SetChangeWeaponType(EnWeaponType targetWeaponType)
{
	m_changeTargetWeaponType = targetWeaponType;
}

void WeaponManager::ChangeSubWeaponTypeToChangeWeaponType()
{
	m_changeTargetWeaponType = m_subWeaponType;
	//一時的に切り替え対象武器のアドレスを保存
	m_tempWeaponType = &m_subWeaponType;
}

void WeaponManager::ChangeSubWeaponType2ToChangeWeaponType()
{
	m_changeTargetWeaponType = m_subWeaponType2;
	//一時的に切り替え対象武器のアドレスを保存
	m_tempWeaponType = &m_subWeaponType2;
}

void WeaponManager::ChangeChangeWeaponTypeToMainWeaponType()
{
	//保存しておいた入れ替え元の変数をメイン武器のものに変更
	*m_tempWeaponType = m_mainWeaponType;
	//メイン武器の中身を切り替え武器のものに変更
	m_mainWeaponType = m_changeTargetWeaponType;

}

void WeaponManager::DeleteAllWeaponInstance()
{
	for (auto& weapon : m_weaponsMap)
	{
		weapon.second->DeleteThis();
	}

	m_weaponsMap.clear();
}


