#pragma once
#include "IWeapon.h"

class Brave;

class WeaponBase:public IWeapon
{
public:
	WeaponBase(){}
	virtual ~WeaponBase(){}

	////////////////////////////////////////////////////////////////////////
	///仮想関数、純粋仮想関数
	////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// モデルの初期化
	/// </summary>
	virtual void InitModel() override{}
	/// <summary>
	/// 武器の移動処理
	/// </summary>
	virtual void MoveWeapon() override{}
	/// <summary>
	/// 武器を装備している時の移動処理
	/// </summary>
	virtual void MoveArmed() override{}
	/// <summary>
	/// 武器を収納している時の移動処理
	/// </summary>
	virtual void MoveStowed() override{}
	/// <summary>
	/// 武器の当たり判定
	/// </summary>
	/// <returns></returns>
	virtual bool IsHitCollision()
	{
		return false;
	}
	/// <summary>
	/// スキル攻撃処理
	/// </summary>
	virtual void ProcessSkillAttack(){}

	/// <summary>
	/// 攻撃やスキル時のジャンプの速度の取得
	/// </summary>
	/// <returns></returns>
	virtual float GetJampSpeed()
	{
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////
	///その他の関数
	///////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 武器の状態ステートを逆の状態に変える。
	/// </summary>
	/// <param name="NowWeaponState">現在の武器の状態ステート</param>
	void ReverseWeaponState();
	/// <summary>
	/// 武器の状態ステートを設定
	/// </summary>
	/// <param name="nextWeaponState"></param>
	void SetWeaponState(EnWeaponState enWeaponState)
	{
		m_enWeaponState = enWeaponState;
	}
	/// <summary>
	/// 武器の状態ステートを返す
	/// </summary>
	/// <returns></returns>
	EnWeaponState& GetWeaponState()
	{
		return m_enWeaponState;
	}

	/// <summary>
	/// 武器の攻撃力を取得
	/// </summary>
	/// <returns></returns>
	const int& GetWeaponPower() const
	{
		return m_power;
	}

	/// <summary>
	/// 収納状態フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetStowedFlag(bool flag)
	{
		m_stowedFlag = flag;
	}
	/// <summary>
	/// 収納状態フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetStowedFlag() const
	{
		return m_stowedFlag;
	}

	/// <summary>
	/// 武器の防御タイプを取得
	/// </summary>
	/// <returns></returns>
	const EnDefendTipe& GetEnDefendTipe() const
	{
		return m_enDefendTipe;
	}
	/// <summary>
	/// 多段ヒット攻撃がヒットしたかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	virtual void SetHittableFlag(bool flag)
	{
	};
	/// <summary>
	/// 多段ヒット攻撃がヒットしたかのフラグを取得
	/// </summary>
	/// <returns></returns>
	virtual const bool& GetHittableFlag() const
	{
		return true;
	}

protected:
	/// <summary>
	/// 武器の防御タイプを設定
	/// </summary>
	/// <returns></returns>
	void SetEnDefendTipe(EnDefendTipe defendTipe)
	{
		m_enDefendTipe = defendTipe;
	}
	/// <summary>
	/// 武器の攻撃力を設定
	/// </summary>
	/// <param name="power"></param>
	void SetWeaponPower(int power)
	{
		m_power = power;
	}

protected:

	Brave* m_brave = nullptr;

	EnWeaponState m_enWeaponState = enWeaponState_None;

	int m_power = 0;		//武器の攻撃力

	EnDefendTipe m_enDefendTipe = enDefendTipe_None;

	bool m_stowedFlag = false;		//収納状態フラグ。処理しないようにするためのフラグ

};

