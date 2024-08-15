#pragma once

#include "WeaponStatus.h"

#include "BraveAnimClipAndStateInfo.h"

class WeaponStatus;
class Brave;

using namespace BraveState;

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
		enArmed,			//装備状態
		enStowed			//収納状態
	};

	/// <summary>
	/// コンボステート
	/// </summary>
	enum EnComboState
	{
		enCombo_None = static_cast<EnComboState>(enBraveState_FirstAttack - 1),//勇者の攻撃ステートに値を合わせる
		enCombo_First = enBraveState_FirstAttack,
		enCombo_Second,
		enCombo_Third,
		enCombo_End,

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
	/// 現在のコンボステートを取得
	/// </summary>
	/// <returns></returns>
	EnComboState GetCurrentComboState()
	{
		return m_enComboState;
	}



	/// <summary>
	/// 収納状態に切り替える
	/// </summary>
	virtual void ChangeStowedState();

	/// <summary>
	/// 装備状態に切り替える
	/// </summary>
	virtual void ChangeArmedState();


	/// <summary>
	/// コンボ攻撃処理を進める
	/// </summary>
	virtual void ProceedComboAttack(){}
	/// <summary>
	/// コンボ攻撃処理をリセットする
	/// </summary>
	virtual void ResetComboAttack(){}

	/// <summary>
	/// 回避、防御アクションを終わるか
	/// </summary>
	/// <returns>終わるならtrue</returns>
	virtual bool IsEndDefensiveAction() { return false; }


protected:

	Brave* m_brave = nullptr;		//プレイヤーのインスタンス

	WeaponStatus m_status;			//ステータス


	EnWeaponState m_enWeaponState = enStowed;			//武器の状態。デフォルトは収納状態


	Vector3 m_stowedPosition = { 0.0f,100.0f,0.0f };		//収納状態時の座標


	EnComboState m_enComboState = enCombo_None;

};

