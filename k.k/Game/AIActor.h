#pragma once
#include "Status.h"


class AIActor:public IGameObject
{
public:
	AIActor();
	virtual ~AIActor();

	/// <summary>
	/// モデルの座標、回転、サイズの設定
	/// </summary>
	/// <param name="modelRender"></param>
	void SetTransFormModel(ModelRender& modelRender);

	/// <summary>
	/// 座標、回転、サイズの設定
	/// </summary>
	/// <param name="position"></param>
	/// <param name="scale"></param>
	/// <param name="rotation"></param>
	void SetTransForm(Vector3 position, Quaternion rotation, Vector3 scale = Vector3::One);

	/// <summary>
	/// 攻撃時の音を再生
	/// </summary>
	virtual void PlayAttackSound();

	/// <summary>
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus()
	{
		return m_status;
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
	/// 回転を設定
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 拡大率の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale() const
	{
		return m_scale;
	}
	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// 前方向を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_forward;
	}

	/// <summary>
	/// 勝利フラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetWinFlag(bool flag)
	{
		m_winFlag = flag;
	}
	/// <summary>
	/// 勝利フラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetWinFlag() const
	{
		return m_winFlag;
	}

	/// <summary>
	/// 移動方向を速度を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

protected:

	/// <summary>
	/// モデルの初期化
	/// </summary>
	virtual void InitModel() = 0;

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	virtual void PlayAnimation() = 0;

	/// <summary>
	/// ステート管理
	/// </summary>
	virtual void ManageState() = 0;

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEnable() const = 0;



	Status						m_status;

	Vector3						m_targetPosition = Vector3::Zero;		//ターゲットの座標を格納

	Vector3						m_toTarget= Vector3::Zero;				//ターゲットに向かうベクトル

	Vector3						m_position = Vector3::Zero;
	Vector3						m_rotMove = Vector3::Zero;
	Vector3						m_moveSpeed = Vector3::Zero;

	Vector3						m_forward = Vector3::Zero;				//前方向

	Vector3						m_SaveMoveSpeed = Vector3::Zero;		//回転のみ使用する時に使う

	Quaternion					m_rotation = Quaternion::Identity;
	Vector3						m_scale = Vector3::One;


	bool						m_winFlag = false;						//勝ったかのフラグ

};

