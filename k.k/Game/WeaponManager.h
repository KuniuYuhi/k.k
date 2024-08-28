#pragma once

#include "WeaponInfo.h"

class WeaponBase;


using namespace WeaponType;

/// <summary>
/// 武器を管理するマネージャークラス。
/// シングルトンパターン
/// </summary>
class WeaponManager:public Noncopyable
{
private:
	WeaponManager();
	~WeaponManager();

public:

	static WeaponManager* m_instance;//唯一のインスタンスのアドレスを記録する変数。

	/// <summary>
	/// インスタンスを生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static WeaponManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="armedWeaponType">最初に装備する武器のタイプ</param>
	void Init(EnWeaponType main, EnWeaponType sub, EnWeaponType sub2);

	/// <summary>
	/// 使う武器の役割を設定する
	/// </summary>
	/// <param name="main">メイン武器のタイプ</param>
	/// <param name="sub">サブ武器１のタイプ</param>
	/// <param name="sub2">サブ武器２にタイプ</param>
	void SettingUseWeaponRole(EnWeaponType main, EnWeaponType sub, EnWeaponType sub2);

	/// <summary>
	/// 装備する武器を切り替える
	/// </summary>
	/// <param name="armedWeaponType">切り替える武器のタイプ</param>
	void ChangeArmedWeapon(EnWeaponType armedWeaponType);



	/// <summary>
	/// 切り替え武器タイプを設定
	/// </summary>
	/// <param name="targetWeaponType">切り替え武器タイプ</param>
	void SetChangeWeaponType(EnWeaponType targetWeaponType);

	/// <summary>
	/// サブ武器１を切り替え武器にする
	/// </summary>
	void ChangeSubWeaponTypeToChangeWeaponType();

	/// <summary>
	/// サブ武器２を切り替え武器にする
	/// </summary>
	void ChangeSubWeaponType2ToChangeWeaponType();

	/// <summary>
	/// 切り替え武器をメイン武器にする
	/// </summary>
	void ChangeChangeWeaponTypeToMainWeaponType();

	/// <summary>
	/// 装備している武器を取得
	/// </summary>
	/// <returns></returns>
	WeaponBase* GetArmedWeapon()
	{
		return m_armedWeapon;
	}



	/// <summary>
	/// メイン武器のタイプを取得
	/// </summary>
	/// <returns></returns>
	EnWeaponType GetMainWeaponType() const
	{
		return m_mainWeaponType;
	}

	/// <summary>
	/// 切り替え武器タイプを取得
	/// </summary>
	/// <returns></returns>
	EnWeaponType GetChangeTargetWeaponType() const
	{
		return m_changeTargetWeaponType;
	}



	



private:

	



	


private:

	std::map<EnWeaponType, WeaponBase*> m_weaponsMap;		//武器を格納しておくマップ

	WeaponBase* m_armedWeapon = nullptr;

	EnWeaponType m_mainWeaponType;		//メイン武器タイプ
	EnWeaponType m_subWeaponType;		//サブ武器1
	EnWeaponType m_subWeaponType2;		//サブ武器2
	EnWeaponType m_changeTargetWeaponType;	//切り替え対象の武器タイプ

	EnWeaponType* m_tempWeaponType;

};

