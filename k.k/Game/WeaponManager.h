#pragma once

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


private:



};

