#pragma once
#include "MyWeapon.h"
#include "WeaponBase.h"


class SwordShield;
class BigSword;

/*
*@brief 武器の管理 
*/
class WeaponManager :public Noncopyable
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
	/// インスタンスの取得。
	/// ウェポンマネージャーの関数を使うときはこの関数を経由する
	/// </summary>
	/// <returns></returns>
	static WeaponManager* GetInstance()
	{
		return m_weaponInstance;
	}

	/// <summary>
	/// 武器の生成
	/// </summary>
	/// <param name="weaponTipe"></param>
	/// <returns></returns>
	WeaponBase* CreateWeapon(EnWeaponType weaponTipe);

	/// <summary>
	/// メイン武器とサブ武器の入れ替え
	/// </summary>
	/// <param name="mainWeapon">メイン武器</param>
	/// <param name="subWeapon">サブ武器</param>
	void SwapWeapons(WeaponBase* mainWeapon, WeaponBase* subWeapon);
	

	/// <summary>
	/// メイン武器の種類を設定
	/// </summary>
	/// <param name="mainWeaponTipe"></param>
	void SetMainWeapon(const EnWeaponType mainWeaponTipe)
	{
		m_selectMainWeaponType = mainWeaponTipe;
		m_mainWeapon.m_enWeaponType = m_selectMainWeaponType;
	}
	/// <summary>
	/// サブ武器の種類を設定
	/// </summary>
	/// <param name="subWeaponTipe"></param>
	void SetSubWeapon(const EnWeaponType subWeaponTipe)
	{
		m_selectSubWeaponType = subWeaponTipe;
		m_subWeapon.m_enWeaponType = m_selectSubWeaponType;
	}
	/// <summary>
	/// メイン武器の種類の取得
	/// </summary>
	/// <returns></returns>
	EnWeaponType& GetMainWeapon()
	{
		return m_selectMainWeaponType;
	}
	/// <summary>
	/// サブ武器の種類の取得
	/// </summary>
	/// <returns></returns>
	EnWeaponType& GetSubWeapon()
	{
		return m_selectSubWeaponType;
	}

private:
	/// <summary>
	/// 剣と盾のオブジェクトの生成
	/// </summary>
	/// <returns></returns>
	WeaponBase* CreateSwordShield();
	/// <summary>
	/// 両手剣のオブジェクトの生成
	/// </summary>
	/// <returns></returns>
	WeaponBase* CreateBigSword();
	/// <summary>
	/// 弓のオブジェクトの生成
	/// </summary>
	/// <returns></returns>
	WeaponBase* CreateBow();

private:

	struct WeaponInfo
	{
		WeaponBase* m_useWeapon = nullptr;
		EnWeaponType m_enWeaponType;
	};

	//std::vector<WeaponInfo*> m_weaponMap;	//武器の種類とインスタンスを関連づけて格納
													//格納するのは基本的にメイン武器とサブ武器のみ

	WeaponInfo m_mainWeapon;										//メイン武器
	WeaponInfo m_subWeapon;											//サブ武器

	 EnWeaponType m_selectMainWeaponType = enWeaponType_Num;		//メイン武器(一つめの選択)
	 EnWeaponType m_selectSubWeaponType = enWeaponType_Num;			//サブ武器(二つめの選択)


	static WeaponManager* m_weaponInstance;							//唯一のインスタンスのアドレスを記録する変数。


};




