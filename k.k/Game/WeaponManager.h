#pragma once
#include "MyWeapon.h"
#include "WeaponBase.h"


class SwordShield;
class BigSword;

/*
*@brief ����̊Ǘ� 
*/
class WeaponManager :public Noncopyable
{
private:
	WeaponManager();
	~WeaponManager();
public:
	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance()
	{
		m_weaponInstance = new WeaponManager;
	}
	/// <summary>
	/// �C���X�^���X�̔j���B
	/// </summary>
	static void DeleteInstance()
	{
		delete m_weaponInstance;
		m_weaponInstance = nullptr;
	}
	/// <summary>
	/// �C���X�^���X�̎擾�B
	/// �E�F�|���}�l�[�W���[�̊֐����g���Ƃ��͂��̊֐����o�R����
	/// </summary>
	/// <returns></returns>
	static WeaponManager* GetInstance()
	{
		return m_weaponInstance;
	}

	/// <summary>
	/// ����̐���
	/// </summary>
	/// <param name="weaponTipe"></param>
	/// <returns></returns>
	WeaponBase* CreateWeapon(EnWeaponType weaponTipe);

	/// <summary>
	/// ���C������ƃT�u����̓���ւ�
	/// </summary>
	/// <param name="mainWeapon">���C������</param>
	/// <param name="subWeapon">�T�u����</param>
	void SwapWeapons(WeaponBase* mainWeapon, WeaponBase* subWeapon);
	

	/// <summary>
	/// ���C������̎�ނ�ݒ�
	/// </summary>
	/// <param name="mainWeaponTipe"></param>
	void SetMainWeapon(const EnWeaponType mainWeaponTipe)
	{
		m_selectMainWeaponType = mainWeaponTipe;
		m_mainWeapon.m_enWeaponType = m_selectMainWeaponType;
	}
	/// <summary>
	/// �T�u����̎�ނ�ݒ�
	/// </summary>
	/// <param name="subWeaponTipe"></param>
	void SetSubWeapon(const EnWeaponType subWeaponTipe)
	{
		m_selectSubWeaponType = subWeaponTipe;
		m_subWeapon.m_enWeaponType = m_selectSubWeaponType;
	}
	/// <summary>
	/// ���C������̎�ނ̎擾
	/// </summary>
	/// <returns></returns>
	EnWeaponType& GetMainWeapon()
	{
		return m_selectMainWeaponType;
	}
	/// <summary>
	/// �T�u����̎�ނ̎擾
	/// </summary>
	/// <returns></returns>
	EnWeaponType& GetSubWeapon()
	{
		return m_selectSubWeaponType;
	}

private:
	/// <summary>
	/// ���Ə��̃I�u�W�F�N�g�̐���
	/// </summary>
	/// <returns></returns>
	WeaponBase* CreateSwordShield();
	/// <summary>
	/// ���茕�̃I�u�W�F�N�g�̐���
	/// </summary>
	/// <returns></returns>
	WeaponBase* CreateBigSword();
	/// <summary>
	/// �|�̃I�u�W�F�N�g�̐���
	/// </summary>
	/// <returns></returns>
	WeaponBase* CreateBow();

private:

	struct WeaponInfo
	{
		WeaponBase* m_useWeapon = nullptr;
		EnWeaponType m_enWeaponType;
	};

	//std::vector<WeaponInfo*> m_weaponMap;	//����̎�ނƃC���X�^���X���֘A�Â��Ċi�[
													//�i�[����̂͊�{�I�Ƀ��C������ƃT�u����̂�

	WeaponInfo m_mainWeapon;										//���C������
	WeaponInfo m_subWeapon;											//�T�u����

	 EnWeaponType m_selectMainWeaponType = enWeaponType_Num;		//���C������(��߂̑I��)
	 EnWeaponType m_selectSubWeaponType = enWeaponType_Num;			//�T�u����(��߂̑I��)


	static WeaponManager* m_weaponInstance;							//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B


};




