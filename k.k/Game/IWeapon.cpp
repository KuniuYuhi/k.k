#include "stdafx.h"
#include "IWeapon.h"

IWeapon::IWeapon()
{
}

IWeapon::~IWeapon()
{
}

void IWeapon::ReverseWeaponState()
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
