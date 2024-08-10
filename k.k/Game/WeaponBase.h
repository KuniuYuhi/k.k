#pragma once

#include "WeaponStatus.h"

class WeaponStatus;
class Brave;

/// <summary>
/// 武器の基底クラス
/// </summary>
class WeaponBase:public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="brave"></param>
	WeaponBase(Brave* brave)
	{
		m_brave = brave;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~WeaponBase() = default;




	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack() = 0;


protected:

	Brave* m_brave = nullptr;		//プレイヤーのインスタンス

	WeaponStatus m_status;			//ステータス




};

