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
	/// デストラクタ
	/// </summary>
	virtual ~WeaponBase() = default;

	/// <summary>
	/// 武器の状態
	/// </summary>
	enum EnWeaponState
	{
		enArmed,
		enStowed
	};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void AttackAction() = 0;



	/// <summary>
	/// 現在の武器の状態を設定
	/// </summary>
	/// <param name="setWeaponState"></param>
	void SetCurrentWeaponState(EnWeaponState setWeaponState)
	{
		m_enWeaponState = setWeaponState;
	}

	/// <summary>
	/// 現在の武器の状態を取得
	/// </summary>
	/// <returns></returns>
	EnWeaponState GetCurrentWeaponState()
	{
		return m_enWeaponState;
	}


	/// <summary>
	/// 収納状態に切り替える
	/// </summary>
	virtual void ChangeStowedState();

	/// <summary>
	/// 装備状態に切り替える
	/// </summary>
	virtual void ChangeArmedState();



protected:

	Brave* m_brave = nullptr;		//プレイヤーのインスタンス

	WeaponStatus m_status;			//ステータス


	EnWeaponState m_enWeaponState = enStowed;			//武器の状態。デフォルトは収納状態


	Vector3 m_stowedPosition = { 0.0f,100.0f,0.0f };		//収納状態時の座標




};

