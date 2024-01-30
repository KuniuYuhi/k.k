#include "stdafx.h"
#include "WeaponBase.h"

void WeaponBase::StartSetWeaponState(EnWeaponState enWeaponState)
{
	SetWeaponState(enWeaponState);
	//������ԂȂ�
	if (GetWeaponState() == enWeaponState_Armed)
	{
		//���[��ԃt���O���Ȃ��ɂ���
		SetStowedFlag(false);
	}
	else
	{
		SetStowedFlag(true);
	}
}

void WeaponBase::ReverseWeaponState()
{
	//���݂̕���̏�ԃX�e�[�g��������ԂȂ���[��ԃX�e�[�g�ɕύX����
	if (m_enWeaponState == enWeaponState_Armed)
	{
		m_enWeaponState = enWeaponState_Stowed;
	}
	//���݂̕���̏�ԃX�e�[�g�����[��ԂȂ瑕����ԃX�e�[�g�ɕύX����
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
	//�ϋv�l���Ȃ��Ȃ����Ƃ��̏���
	if (m_status.GetEndurance() <= 0)
	{
		ProcessNoEndurance();
		return;
	}
	//�ϋv�l��0����0�ȏ�ɉ񕜂�����
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
