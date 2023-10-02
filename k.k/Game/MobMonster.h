#pragma once
#include "AIActor.h"

class Lich;

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
	Vector3 SetRamdomDirection(int range);

	/// <summary>
	/// 壁(森)にぶつかったか
	/// </summary>
	/// <returns></returns>
	virtual bool IsBumpedForest(float pos2Length);

	/// <summary>
	/// モンスターにぶつかったか
	/// </summary>
	/// <returns></returns>
	bool IsBumpedMonster();

	/// <summary>
	/// はじきパワーを小さくする
	/// </summary>
	void SubPassPower();

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

	/// <summary>
	/// やられたときの処理
	/// </summary>
	virtual void Dead();

	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name="rotSpeed">回転する速度(移動)</param>
	/// <param name="rotOnlySpeed">回転する速度(移動しない時)</param>
	/// <returns></returns>
	//Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	/// <summary>
	/// ヒットエフェクト生成
	/// </summary>
	void CreateHitEffect();

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


	Lich*					m_lich = nullptr;

	Vector3					m_passPower = g_vec3One;		//はじくときの力

	Vector3					m_direction = Vector3::Zero;	//移動する方向ベクトル

	float					m_angleChangeTime = 0.0f;		//向かうベクトルを計算するタイマー

	float					m_distanceToPlayer = 0.0f;		//プレイヤーとの距離
	float					m_attackRange = 0.0f;			//攻撃できる範囲
	float					m_stayRange = 0.0f;				//移動しない範囲

	float					m_ramdomAngle = 0.0f;			//ランダムなアングル。移動方向を決めるときに使う

	float					m_attackIntervalTime = 0.0f;	//攻撃した後のインターバル

	int						m_angleRange = 2;				//移動するアングルの範囲

	float					m_pos2Length = 0.0;				//壁と判

	bool                    m_chasePlayerFlag = false;		//プレイヤーを追いかけるかのフラグ
};

