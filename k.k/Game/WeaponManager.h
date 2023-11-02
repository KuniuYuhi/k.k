#pragma once
#include "MyWeapon.h"
#include "IWeapon.h"

class SwordShield;
class BigSword;

/*
*@brief 武器の管理 
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
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_weaponInstance = new WeaponManager;
	}
	/// <summary>
	/// インスタンスの破棄。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_weaponInstance;
		m_weaponInstance = nullptr;
	}

	/// <summary>
	/// マップに全ての武器のインスタンスを代入
	/// </summary>
	void AddToWeaponMap();

	/// <summary>
	/// 武器の生成
	/// </summary>
	/// <param name="weaponTipe">生成したい武器の種類</param>
	template<class T>
	T* CreateWeapon(EnWeaponType weaponTipe);

	/// <summary>
	/// 武器の検索のヘルパー関数
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="weaponTipe">検索したい武器の種類</param>
	/// <returns></returns>
	template<class T>
	T* FindGOWeapon(EnWeaponType weaponTipe)
	{
		//武器が見つかったら
		if (m_weaponMap.find(weaponTipe) != m_weaponMap.end())
		{
			return m_weaponMap[weaponTipe];
		}
		//見つからないなら
		return nullptr;
		
	}


	/// <summary>
	/// メイン武器の種類を設定
	/// </summary>
	/// <param name="mainWeaponTipe"></param>
	static void SetMainWeapon(EnWeaponType mainWeaponTipe)
	{
		m_selectMainWeapon = mainWeaponTipe;
	}
	/// <summary>
	/// サブ武器の種類を設定
	/// </summary>
	/// <param name="subWeaponTipe"></param>
	static void SetSubWeapon(EnWeaponType subWeaponTipe)
	{
		m_selectSubWeapon = subWeaponTipe;
	}
	/// <summary>
	/// メイン武器の種類の取得
	/// </summary>
	/// <returns></returns>
	static const EnWeaponType& GetMainWeapon()
	{
		return m_selectMainWeapon;
	}
	/// <summary>
	/// サブ武器の種類の取得
	/// </summary>
	/// <returns></returns>
	static const EnWeaponType& GetSubWeapon()
	{
		return m_selectSubWeapon;
	}

public:

	std::map<EnWeaponType, IWeapon*> m_weaponMap;	//武器の種類とインスタンスを関連づけて格納
													//格納するのは基本的にメイン武器とサブ武器のみ

	
	SwordShield* m_swordShield = nullptr;
	BigSword* m_bigSword = nullptr;
	
	static EnWeaponType m_selectMainWeapon;					//メイン武器(一つめの選択)
	static EnWeaponType m_selectSubWeapon;					//サブ武器(二つめの選択)
	

	static WeaponManager* m_weaponInstance;		//唯一のインスタンスのアドレスを記録する変数。

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
