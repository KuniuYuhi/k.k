#include "stdafx.h"
#include "WeaponBase.h"

void WeaponBase::StartSetWeaponState(EnWeaponState enWeaponState)
{
	SetWeaponState(enWeaponState);
	//装備状態なら
	if (GetWeaponState() == enWeaponState_Armed)
	{
		//収納状態フラグをなしにする
		SetStowedFlag(false);
	}
	else
	{
		SetStowedFlag(true);
	}
}

void WeaponBase::ReverseWeaponState()
{
	//現在の武器の状態ステートが装備状態なら収納状態ステートに変更する
	if (m_enWeaponState == enWeaponState_Armed)
	{
		m_enWeaponState = enWeaponState_Stowed;
	}
	//現在の武器の状態ステートが収納状態なら装備状態ステートに変更する
	else if (m_enWeaponState == enWeaponState_Stowed)
	{
		SetStowedFlag(false);
		m_enWeaponState = enWeaponState_Armed;
	}
}
