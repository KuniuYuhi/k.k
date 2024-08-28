#pragma once

#include "WeaponInfo.h"

class WeaponBase;


using namespace WeaponType;

/// <summary>
/// ������Ǘ�����}�l�[�W���[�N���X�B
/// �V���O���g���p�^�[��
/// </summary>
class WeaponManager:public Noncopyable
{
private:
	WeaponManager();
	~WeaponManager();

public:

	static WeaponManager* m_instance;//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

	/// <summary>
	/// �C���X�^���X�𐶐�
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X���擾�B
	/// </summary>
	/// <returns></returns>
	static WeaponManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// �C���X�^���X���폜
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="armedWeaponType">�ŏ��ɑ������镐��̃^�C�v</param>
	void Init(EnWeaponType main, EnWeaponType sub, EnWeaponType sub2);

	/// <summary>
	/// �g������̖�����ݒ肷��
	/// </summary>
	/// <param name="main">���C������̃^�C�v</param>
	/// <param name="sub">�T�u����P�̃^�C�v</param>
	/// <param name="sub2">�T�u����Q�Ƀ^�C�v</param>
	void SettingUseWeaponRole(EnWeaponType main, EnWeaponType sub, EnWeaponType sub2);

	/// <summary>
	/// �������镐���؂�ւ���
	/// </summary>
	/// <param name="armedWeaponType">�؂�ւ��镐��̃^�C�v</param>
	void ChangeArmedWeapon(EnWeaponType armedWeaponType);



	/// <summary>
	/// �؂�ւ�����^�C�v��ݒ�
	/// </summary>
	/// <param name="targetWeaponType">�؂�ւ�����^�C�v</param>
	void SetChangeWeaponType(EnWeaponType targetWeaponType);

	/// <summary>
	/// �T�u����P��؂�ւ�����ɂ���
	/// </summary>
	void ChangeSubWeaponTypeToChangeWeaponType();

	/// <summary>
	/// �T�u����Q��؂�ւ�����ɂ���
	/// </summary>
	void ChangeSubWeaponType2ToChangeWeaponType();

	/// <summary>
	/// �؂�ւ���������C������ɂ���
	/// </summary>
	void ChangeChangeWeaponTypeToMainWeaponType();

	/// <summary>
	/// �������Ă��镐����擾
	/// </summary>
	/// <returns></returns>
	WeaponBase* GetArmedWeapon()
	{
		return m_armedWeapon;
	}



	/// <summary>
	/// ���C������̃^�C�v���擾
	/// </summary>
	/// <returns></returns>
	EnWeaponType GetMainWeaponType() const
	{
		return m_mainWeaponType;
	}

	/// <summary>
	/// �؂�ւ�����^�C�v���擾
	/// </summary>
	/// <returns></returns>
	EnWeaponType GetChangeTargetWeaponType() const
	{
		return m_changeTargetWeaponType;
	}



	



private:

	



	


private:

	std::map<EnWeaponType, WeaponBase*> m_weaponsMap;		//������i�[���Ă����}�b�v

	WeaponBase* m_armedWeapon = nullptr;

	EnWeaponType m_mainWeaponType;		//���C������^�C�v
	EnWeaponType m_subWeaponType;		//�T�u����1
	EnWeaponType m_subWeaponType2;		//�T�u����2
	EnWeaponType m_changeTargetWeaponType;	//�؂�ւ��Ώۂ̕���^�C�v

	EnWeaponType* m_tempWeaponType;

};

