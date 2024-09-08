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
	//������}�b�v�Ɋi�[����
	m_weaponsMap.insert(std::make_pair(enSwordShield, NewGO<SwordShield>(0, "SwordShield")));
	m_weaponsMap.insert(std::make_pair(enGreateSword, NewGO<GreateSword>(0, "GreateSword")));
	m_weaponsMap.insert(std::make_pair(enBowArrow, NewGO<Bow>(0, "Bow")));

	//�S�Ă̕���̏�Ԃ����[��Ԃɂ���
	for (auto& weapon : m_weaponsMap)
	{
		weapon.second->ChangeStowedState();
	}
	
	//����̖�����ݒ肷��
	SettingUseWeaponRole(main, sub, sub2);

	//���C������̃L�[�ɑΉ�����v�f���擾�B���݂̕���Ƃ���
	m_armedWeapon = m_weaponsMap.at(m_mainWeaponType);
	//����𑕔���Ԃɂ���
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
	//���ݑ������Ă��镐������[��Ԃɂ���
	m_armedWeapon->ChangeStowedState();

	//�������镐����}�b�v���玝���Ă���
	m_armedWeapon = m_weaponsMap.at(armedWeaponType);
	//�}�b�v�ɂȂ����
	if (m_armedWeapon == nullptr)
	{
		std::abort();
	}
	//����𑕔���Ԃɂ���
	m_armedWeapon->ChangeArmedState();

	//���C������̒��g��؂�ւ��Ώە���ɐ؂�ւ���
	ChangeChangeWeaponTypeToMainWeaponType();
}

void WeaponManager::SetChangeWeaponType(EnWeaponType targetWeaponType)
{
	m_changeTargetWeaponType = targetWeaponType;
}

void WeaponManager::ChangeSubWeaponTypeToChangeWeaponType()
{
	m_changeTargetWeaponType = m_subWeaponType;
	//�ꎞ�I�ɐ؂�ւ��Ώە���̃A�h���X��ۑ�
	m_tempWeaponType = &m_subWeaponType;
}

void WeaponManager::ChangeSubWeaponType2ToChangeWeaponType()
{
	m_changeTargetWeaponType = m_subWeaponType2;
	//�ꎞ�I�ɐ؂�ւ��Ώە���̃A�h���X��ۑ�
	m_tempWeaponType = &m_subWeaponType2;
}

void WeaponManager::ChangeChangeWeaponTypeToMainWeaponType()
{
	//�ۑ����Ă���������ւ����̕ϐ������C������̂��̂ɕύX
	*m_tempWeaponType = m_mainWeaponType;
	//���C������̒��g��؂�ւ�����̂��̂ɕύX
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


