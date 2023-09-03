#pragma once
#include "AIActor.h"

class MobMonster:public AIActor
{
public:

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="charaCon">移動させたいオブジェクトのキャラクターコントローラー</param>
	void Move(CharacterController& charaCon);

	/// <summary>
	/// 向かうベクトルを設定
	/// </summary>
	/// <param name="range"></param>
	/// <returns></returns>
	Vector3 SetDirection(int range);

	/// <summary>
	/// 壁(森)にぶつかったか
	/// </summary>
	/// <returns></returns>
	bool IsBumpedForest();

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const = 0;

	/// <summary>
	/// 回転可能か
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;

	/// <summary>
	/// 攻撃可能か
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEntable() const = 0;

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	virtual void Damage(int attack) = 0;

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
	/// 回転のみを行う処理条件
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly();



	Vector3 m_direction = Vector3::Zero;

	float					m_angleChangeTime = 0.0f;					//向かうベクトルを計算するタイマー

	const float				m_distanceToPlayer = 0.0f;				//プレイヤーとの距離
	const float				m_attackRange = 0.0f;						//攻撃できる範囲
	const float				m_stayDistance = 0.0f;						//移動しない距離

	float					m_ramdomAngle = 0.0f;						//

	const float				m_attackIntervalTime = 0.0f;				//攻撃した後のインターバル

	const int				m_angleRange = 2;					//移動するアングルの範囲
};

