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
