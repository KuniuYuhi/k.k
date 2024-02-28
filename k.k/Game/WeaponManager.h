#pragma once
#include "MyWeapon.h"
#include "WeaponBase.h"


class SwordShield;
class GreatSword;

/*
*@brief ����̊Ǘ� 
*/
class WeaponManager :public Noncopyable
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WeaponManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~WeaponManager();
public:



	/// <summary>
	///	����̏��
	/// </summary>
	struct UseWeaponInfo
	{
		WeaponBase* weapon = nullptr;	//����I�u�W�F�N�g
		int AnimationStartIndexNo = 0;	//����̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	};

	/// <summary>
	/// ����}�l�[�W���[�C���X�^���X�𐶐�
	/// </summary>
	static void CreateInstance()
	{
		m_weaponInstance = new WeaponManager();
	}
	/// <summary>
	/// ����}�l�[�W���[�C���X�^���X���폜
	/// </summary>
	static void DeleteInstance()
	{
		delete m_weaponInstance;
		m_weaponInstance = nullptr;
	}
	/// <summary>
	/// ����}�l�[�W���[�C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static WeaponManager* GetInstance()
	{
		return m_weaponInstance;
	}


	/// <summary>
	/// �S�Ă̕����������
	/// </summary>
	/// <param name="animClipNum"></param>
	void InitAllWeapon(int animClipNum);
	/// <summary>
	/// �S�Ă̕���𐶐�
	/// </summary>
	void CreateAllWeapon();

	/// <summary>
	/// ����̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ݒ�
	/// </summary>
	/// <param name="animClipNum"></param>
	/// <param name="setWeapon"></param>
	void SetWeaponAnimationStartIndexNo(
		int animClipNum, EnMaxWeapons setWeapon);

	//����̐؂�ւ�

	//�A�j���[�V�����N���b�v�̍ŏ��̔ԍ��̓���ւ�
	

	/// <summary>
	/// ����̃I�u�W�F�N�g���擾
	/// </summary>
	/// <param name="getWeaponType">����̎��</param>
	/// <returns></returns>
	WeaponBase* GetWeaponObject(EnWeaponType getWeaponType);
	
	/// <summary>
	/// ����̃I�u�W�F�N�g���擾
	/// </summary>
	/// <param name="getWeaponType">����̃��C����T�u</param>
	/// <returns></returns>
	WeaponBase* GetWeaponObject(EnMaxWeapons getWeapon);

	/// <summary>
	/// ����̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ����擾
	/// </summary>
	/// <param name="getWeapon"></param>
	/// <returns></returns>
	const int GetWeaponAnimationStartIndexNo(EnMaxWeapons getWeapon)
	{
		return m_useWeapon[getWeapon].AnimationStartIndexNo;
	}


	//����̎擾

private:
	
	


private:

	//�g������̏��B����̐��B���C���A�T�u
	UseWeaponInfo m_useWeapon[enMaxWeapons_num];

	static WeaponManager* m_weaponInstance;
};




