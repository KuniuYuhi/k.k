#pragma once
#include "MyWeapon.h"
#include "WeaponBase.h"


class SwordShield;
class GreatSword;

/*
*@brief 武器の管理 
*/
class WeaponManager :public Noncopyable
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WeaponManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WeaponManager();
public:



	/// <summary>
	///	武器の情報
	/// </summary>
	struct UseWeaponInfo
	{
		WeaponBase* weapon = nullptr;	//武器オブジェクト
		int AnimationStartIndexNo = 0;	//武器のアニメーションクリップの最初の番号
	};

	/// <summary>
	/// 武器マネージャーインスタンスを生成
	/// </summary>
	static void CreateInstance()
	{
		m_weaponInstance = new WeaponManager();
	}
	/// <summary>
	/// 武器マネージャーインスタンスを削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_weaponInstance;
		m_weaponInstance = nullptr;
	}
	/// <summary>
	/// 武器マネージャーインスタンスを取得
	/// </summary>
	/// <returns></returns>
	static WeaponManager* GetInstance()
	{
		return m_weaponInstance;
	}


	/// <summary>
	/// 全ての武器を初期化
	/// </summary>
	/// <param name="animClipNum"></param>
	void InitAllWeapon(int animClipNum);
	/// <summary>
	/// 全ての武器を生成
	/// </summary>
	void CreateAllWeapon();

	/// <summary>
	/// 武器のアニメーションクリップの最初の番号を設定
	/// </summary>
	/// <param name="animClipNum"></param>
	/// <param name="setWeapon"></param>
	void SetWeaponAnimationStartIndexNo(
		int animClipNum, EnMaxWeapons setWeapon);

	//武器の切り替え

	//アニメーションクリップの最初の番号の入れ替え
	

	/// <summary>
	/// 武器のオブジェクトを取得
	/// </summary>
	/// <param name="getWeaponType">武器の種類</param>
	/// <returns></returns>
	WeaponBase* GetWeaponObject(EnWeaponType getWeaponType);
	
	/// <summary>
	/// 武器のオブジェクトを取得
	/// </summary>
	/// <param name="getWeaponType">武器のメインやサブ</param>
	/// <returns></returns>
	WeaponBase* GetWeaponObject(EnMaxWeapons getWeapon);

	/// <summary>
	/// 武器のアニメーションクリップの最初の番号を取得
	/// </summary>
	/// <param name="getWeapon"></param>
	/// <returns></returns>
	const int GetWeaponAnimationStartIndexNo(EnMaxWeapons getWeapon)
	{
		return m_useWeapon[getWeapon].AnimationStartIndexNo;
	}


	//武器の取得

private:
	
	


private:

	//使う武器の情報。武器の数。メイン、サブ
	UseWeaponInfo m_useWeapon[enMaxWeapons_num];

	static WeaponManager* m_weaponInstance;
};




