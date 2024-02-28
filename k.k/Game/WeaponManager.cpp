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
		//���ɃC���X�^���X����������G���[
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
	//����̐���
	CreateAllWeapon();

	//animClip���g���ăA�j���[�V�����N���b�v�̍ŏ��̔ԍ��ݒ�
	SetWeaponAnimationStartIndexNo(animClipNum, enMaxWeapons_Main);
	SetWeaponAnimationStartIndexNo(animClipNum, enMaxWeapons_Sub);
	SetWeaponAnimationStartIndexNo(animClipNum, enMaxWeapons_Sub2);

}

void WeaponManager::CreateAllWeapon()
{
	//�\�[�h���V�[���h
	SwordShield* swordShield = NewGO<SwordShield>(0, "swordshield");
	m_useWeapon[enMaxWeapons_Main].weapon = swordShield;
	//�O���C�g�\�[�h
	GreatSword* greatSword = NewGO<GreatSword>(0, "greatsword");
	m_useWeapon[enMaxWeapons_Sub].weapon = greatSword;
	//�{�E���A���[
	Bow* bowArrow = NewGO<Bow>(0, "bowarrow");
	m_useWeapon[enMaxWeapons_Sub2].weapon = bowArrow;


}

void WeaponManager::SetWeaponAnimationStartIndexNo(int animClipNum, EnMaxWeapons setWeapon)
{
	//����̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ݒ�
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

