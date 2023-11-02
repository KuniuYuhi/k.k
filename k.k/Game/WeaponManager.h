#pragma once
#include "MyWeapon.h"
#include "IWeapon.h"

class SwordShield;
class BigSword;

/*
*@brief ����̊Ǘ� 
*/
namespace nsK2EngineLow {

}

class WeaponManager:public Noncopyable
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
	/// �}�b�v�ɑS�Ă̕���̃C���X�^���X����
	/// </summary>
	void AddToWeaponMap();

	/// <summary>
	/// ����̐���
	/// </summary>
	/// <param name="weaponTipe">��������������̎��</param>
	template<class T>
	T* CreateWeapon(EnWeaponType weaponTipe);

	/// <summary>
	/// ����̌����̃w���p�[�֐�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="weaponTipe">��������������̎��</param>
	/// <returns></returns>
	template<class T>
	T* FindGOWeapon(EnWeaponType weaponTipe)
	{
		//���킪����������
		if (m_weaponMap.find(weaponTipe) != m_weaponMap.end())
		{
			return m_weaponMap[weaponTipe];
		}
		//������Ȃ��Ȃ�
		return nullptr;
		
	}


	/// <summary>
	/// ���C������̎�ނ�ݒ�
	/// </summary>
	/// <param name="mainWeaponTipe"></param>
	static void SetMainWeapon(EnWeaponType mainWeaponTipe)
	{
		m_selectMainWeapon = mainWeaponTipe;
	}
	/// <summary>
	/// �T�u����̎�ނ�ݒ�
	/// </summary>
	/// <param name="subWeaponTipe"></param>
	static void SetSubWeapon(EnWeaponType subWeaponTipe)
	{
		m_selectSubWeapon = subWeaponTipe;
	}
	/// <summary>
	/// ���C������̎�ނ̎擾
	/// </summary>
	/// <returns></returns>
	static const EnWeaponType& GetMainWeapon()
	{
		return m_selectMainWeapon;
	}
	/// <summary>
	/// �T�u����̎�ނ̎擾
	/// </summary>
	/// <returns></returns>
	static const EnWeaponType& GetSubWeapon()
	{
		return m_selectSubWeapon;
	}

public:

	std::map<EnWeaponType, IWeapon*> m_weaponMap;	//����̎�ނƃC���X�^���X���֘A�Â��Ċi�[
													//�i�[����̂͊�{�I�Ƀ��C������ƃT�u����̂�

	
	SwordShield* m_swordShield = nullptr;
	BigSword* m_bigSword = nullptr;
	
	static EnWeaponType m_selectMainWeapon;					//���C������(��߂̑I��)
	static EnWeaponType m_selectSubWeapon;					//�T�u����(��߂̑I��)
	

	static WeaponManager* m_weaponInstance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

};

template<class T>
T* WeaponManager::CreateWeapon(EnWeaponType weaponTipe)
{
	switch (weaponTipe)
	{
	case enWeaponType_SwordShield:
		m_weaponMap[enWeaponType_SwordShield] =
			NewGO<SwordShield>(0, "swordshield");
		return m_weaponMap[enWeaponType_SwordShield];
		break;
	case enWeaponType_TwoHandSword:
		m_weaponMap[enWeaponType_TwoHandSword] =
			NewGO<BigSword>(0, "bigsword");
		return m_weaponMap[enWeaponType_TwoHandSword];
		break;
	/*case enWeaponType_Bow:
		return nullptr;
		break;
	case enWeaponType_DoubleSwords:
		return nullptr;
		break;*/
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
