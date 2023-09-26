#pragma once

/// <summary>
/// マジックボールクラス。～ボールの基底クラス
/// </summary>
class MagicBall:public IGameObject
{
public:
	MagicBall();
	virtual ~MagicBall();

	/// <summary>
	/// 直進移動処理
	/// </summary>
	void MoveStraight();

	/// <summary>
	/// 追尾移動処理
	/// </summary>
	/// <param name="targetPosition">追尾対象の座標</param>
	void MoveChase(Vector3 targetPosition);

	/// <summary>
	/// 移動可能時間タイマーの計算
	/// </summary>
	/// <param name="moveLimitTime">制限時間</param>
	void CalcMoveTime(const float moveLimitTime);

	/// <summary>
	/// 生成時の設定
	/// </summary>
	/// <param name="position">生成するキャラの座標</param>
	/// <param name="rotation">生成するキャラの回転</param>
	void Setting(Vector3 position, Quaternion rotation)
	{
		m_position = position;
		m_rotation = rotation;
	}

	/// <summary>
	/// 移動方向の設定
	/// </summary>
	void SetMoveSpeed()
	{
		m_moveSpeed = Vector3::AxisZ;
		m_rotation.Apply(m_moveSpeed);
	}

	virtual void Explosion();


	/// <summary>
	/// 敵に当たったかのフラグを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetHitFlag(bool flag)
	{
		m_hitFlag = flag;
	}

	/// <summary>
	/// 派生クラスの攻撃力を返す
	/// </summary>
	/// <returns></returns>
	virtual const int& GetBallAtk() const = 0;

	/// <summary>
	/// 攻撃力を返す
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return GetBallAtk();
	}


protected:

	Vector3						m_position = Vector3::Zero;
	Vector3						m_moveSpeed = Vector3::Zero;
	Quaternion					m_rotation = Quaternion::Identity;

	float						m_speed = 0.0f;

	float						m_moveTimer = 0.0f;		//ボールが動く時間を計算するタイマー

	bool						m_hitFlag = false;			//敵に当たったかのフラグ

};

