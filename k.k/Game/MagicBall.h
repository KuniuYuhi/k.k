#pragma once
#include "IAttackObject.h"

/// <summary>
/// マジックボールクラス。～ボールの基底クラス
/// </summary>
class MagicBall:public IAttackObject
{
public:
	MagicBall();
	virtual ~MagicBall();

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
	/// 生成開始座標を設定
	/// </summary>
	/// <param name="startPosition"></param>
	void SetStartPosition(Vector3 startPosition)
	{
		m_startPosition = startPosition;
	}

	/// <summary>
	/// 移動方向の設定
	/// </summary>
	void SetMoveSpeed()
	{
		m_moveSpeed = Vector3::AxisZ;
		m_rotation.Apply(m_moveSpeed);
	}

	/// <summary>
	/// 敵に当たったかのフラグを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetHitFlag(bool flag)
	{
		m_hitFlag = flag;
	}

protected:
	/// <summary>
	/// 爆発処理
	/// </summary>
	virtual void Explosion();

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
	/// 強制的に削除するか。移動距離で削除
	/// </summary>
	/// <param name="currentPosition">現在の座標</param>
	/// <param name="deleteLength">削除する距離の長さ</param>
	/// <returns>trueで削除可能</returns>
	bool IsForceDelete(Vector3 currentPosition, float deleteLength);

protected:

	Vector3						m_startPosition = g_vec3Zero;
	Vector3						m_position = g_vec3Zero;
	Vector3						m_moveSpeed = g_vec3Zero;
	Quaternion					m_rotation = g_quatIdentity;



	float						m_speed = 0.0f;

	float						m_moveTimer = 0.0f;		//ボールが動く時間を計算するタイマー

	bool						m_hitFlag = false;			//敵に当たったかのフラグ

};

