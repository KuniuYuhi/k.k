#pragma once
#include "IWeapon.h"
#include "HitDetection.h"
#include "Status.h"
#include "InitEffect.h"

class Brave;
class HitDetection;

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

	/// <summary>
	/// 被ダメージなどで、正常に戻すはずだった変数を強制的にリセットする
	/// </summary>
	virtual void ResetVariable() {}

	/// <summary>
	/// 耐久値がなくなったらときの処理
	/// </summary>
	virtual void ProcessNoEndurance(){}

	/// <summary>
	/// 耐久値が0より大きくなったときの処理(0から以上になった時)
	/// </summary>
	virtual void ProcessOnEndurance() {}

	/// <summary>
	/// 被ダメージ後の値のリセット。スキルなどが中断されたとき用
	/// </summary>
	virtual void postDamageReset(){}

	///////////////////////////////////////////////////////////////////////////
	///その他の関数
	///////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// バトル開始時の武器の状態の設定
	/// </summary>
	/// <param name="enWeaponState"></param>
	void StartSetWeaponState(EnWeaponState enWeaponState);

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
	void SetHittableFlag(bool flag)
	{
		m_hitDelection.SetHittableFlag(flag);
	}
	/// <summary>
	/// 多段ヒット攻撃がヒットしたかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetHittableFlag() const
	{
		return m_hitDelection.GetHittableFlag();
	}

	/// <summary>
	/// 回転可能フラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetRotationDelectionFlag(bool flag)
	{
		m_RotationDelectionFlag = flag;
	}
	/// <summary>
	/// 回転可能フラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetRotationDelectionFlag() const
	{
		return m_RotationDelectionFlag;
	}

	/// <summary>
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus() const
	{
		return m_status;
	}

	/// <summary>
	/// 耐久値の増減の計算。
	/// </summary>
	/// <param name="value"></param>
	/// <param name="addOrSubFlag"></param>
	void CalcEndurance(int value, bool addOrSubFlag);

	/// <summary>
	/// 防御可能かフラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsDefendEnableFlag(bool flag)
	{
		m_isDefendEnableFlag = flag;
	}
	/// <summary>
	/// 防御可能かフラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDefendEnableFlag() const
	{
		return m_isDefendEnableFlag;
	}
	/// <summary>
	/// 攻撃可能かのフラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsAttackEnableFlag(bool flag)
	{
		m_isAttackEnableFlag = flag;
	}
	/// <summary>
	/// 攻撃可能かのフラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsAttackEnableFlag() const
	{
		return m_isAttackEnableFlag;
	}

	/// <summary>
	/// 耐久値が0より大きいかどうか判定
	/// </summary>
	/// <returns>trueで攻撃可能、falseで攻撃不可能</returns>
	const bool& IsWeaponEndurance() const
	{
		if (m_status.GetEndurance() > 0)
		{
			return true;
		}
		return false;
	}

	/// <summary>
	/// 前進するスピードを設定
	/// </summary>
	/// <param name="Speed"></param>
	void SetMoveForwardSpeed(float Speed)
	{
		m_moveForwardSpeed = Speed;
	}
	/// <summary>
	/// 前進するスピードを取得 
	/// </summary>
	/// <returns></returns>
	const float& GetMoveForwardSpeed() const
	{
		return m_moveForwardSpeed;
	}

	/// <summary>
	/// 後退するスピードを設定
	/// </summary>
	/// <param name="Speed"></param>
	void SetMoveBackSpeed(float Speed)
	{
		m_moveForwardSpeed = Speed;
	}
	/// <summary>
	/// 後退するスピードを取得
	/// </summary>
	/// <returns></returns>
	const float& GetMoveBackSpeed() const
	{
		return m_moveForwardSpeed;
	}

	/// <summary>
	/// ノックバックパワーの取得
	/// </summary>
	/// <param name="num">取得したいパワーの番号</param>
	/// <returns></returns>
	const float& GetKnockBackPower(int num);


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

	void PlayEffect(
		InitEffect::EnEFK enEffect,
		Vector3 position,
		float size,
		Quaternion rotation = g_quatIdentity
	);
	

protected:

	enum EnKnockBackPowerNumber
	{
		enKnockBackPowerNumber_None,
		enKnockBackPowerNumber_1combo,
		enKnockBackPowerNumber_2combo,
		enKnockBackPowerNumber_3combo,
		enKnockBackPowerNumber_Skill,
	};

	Status              m_status;
	Brave*				m_brave = nullptr;

	HitDetection		m_hitDelection;

	EnWeaponState		m_enWeaponState = enWeaponState_None;

	EnDefendTipe		m_enDefendTipe = enDefendTipe_None;

	int					m_power = 0;							//武器の攻撃力

	bool				m_stowedFlag = false;					//収納状態フラグ。処理しないようにするためのフラグ

	bool				m_RotationDelectionFlag = false;		//回転可能フラグ

	bool				m_isDefendEnableFlag = true;			//防御可能かのフラグ。trueで可能

	bool				m_isAttackEnableFlag = true;			//攻撃可能かのフラグ。trueで可能

	float				m_moveForwardSpeed = 2.0f;				//前進するときのスピード

	float				m_moveBackSpeed = 1.0f;					//後退するときのスピード

	float m_knockBackSpeed = 1.0f;
};

