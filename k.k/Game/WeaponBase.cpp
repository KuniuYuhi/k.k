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

void WeaponBase::CalcEndurance(int value, bool addOrSubFlag)
{
	int old = m_status.GetEndurance();
	m_status.CalcEndurance(value, addOrSubFlag);
	//耐久値がなくなったときの処理
	if (m_status.GetEndurance() <= 0)
	{
		ProcessNoEndurance();
		return;
	}
	//耐久値が0から0以上に回復したら
	if (old <= 0 && m_status.GetEndurance() > 0)
	{
		ProcessOnEndurance();
	}
}

const float& WeaponBase::GetKnockBackPower(int num)
{
	switch (num)
	{
	case enKnockBackPowerNumber_1combo:
		return m_knockPower_1combo;
	case enKnockBackPowerNumber_2combo:
		return m_knockPower_2combo;
	case enKnockBackPowerNumber_3combo:
		return m_knockPower_3combo;
	case enKnockBackPowerNumber_Skill:
		return m_knockPower_Skill;
	default:
		std::abort();
		break;
	}
}

void WeaponBase::PlayEffect(
	InitEffect::EnEFK enEffect, 
	Vector3 position, float size, Quaternion rotation)
{
	EffectEmitter* effect = NewGO<EffectEmitter>(0);
	effect->Init(enEffect);
	effect->Play();
	effect->SetPosition(position);
	effect->SetScale(g_vec3One * size);
	effect->SetRotation(rotation);
	effect->Update();
}
