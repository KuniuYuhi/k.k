#pragma once

class Brave;

/// <summary>
/// インターフェースの実装
/// </summary>
class IWeapon:public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	IWeapon();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IWeapon();

	/// <summary>
	/// 武器の状態ステート
	/// </summary>
	enum EnWeaponState
	{
		enWeaponState_Stowed,		//収納している状態
		enWeaponState_Armed,		//装備している状態
		enWeaponState_None,			//何も装備していない
	};
	/// <summary>
	/// 武器の防御のタイプ。
	/// </summary>
	enum EnDefendTipe
	{
		enDefendTipe_Defence,	//盾などで防ぐタイプ
		enDefendTipe_avoid,		//回避タイプ
		enDefendTipe_None
	};

	////////////////////////////////////////////////////////////////////////
	///仮想関数、純粋仮想関数
	////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// モデルの初期化
	/// </summary>
	virtual void InitModel() = 0;
	/// <summary>
	/// 武器の移動処理
	/// </summary>
	virtual void MoveWeapon() = 0;
	/// <summary>
	/// 武器を装備している時の移動処理
	/// </summary>
	virtual void MoveArmed() = 0;
	/// <summary>
	/// 武器を収納している時の移動処理
	/// </summary>
	virtual void MoveStowed() = 0;

	/// <summary>
	/// 武器の当たり判定
	/// </summary>
	/// <returns></returns>
	virtual bool IsHitCollision()
	{
		return false;
	};
	/// <summary>
	/// スキル攻撃処理
	/// </summary>
	virtual void ProcessSkillAttack()
	{
	}

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
	/// 武器の攻撃力の設定
	/// </summary>
	/// <param name="power"></param>
	void SetPower(int power)
	{
		m_power = power;
	}
	/// <summary>
	/// 武器の攻撃力を取得
	/// </summary>
	/// <returns></returns>
	const int& GetPower() const
	{
		return m_power;
	}

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
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

	EnDefendTipe m_enDefendTipe = enDefendTipe_None;

	int m_power = 0;		//武器の攻撃力

	Vector3 m_position = g_vec3Zero;	//自身の座標

	bool m_stowedFlag = false;		//収納状態フラグ。処理しないようにするためのフラグ


	//派生クラスで必要なもの
	//ワールド座標
	//武器を持たせるボーンID
	//武器をしまった時のボーンID
};

