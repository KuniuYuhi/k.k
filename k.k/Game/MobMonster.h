#pragma once
#include "MonsterBase.h"
#include "KnockBack.h"


#include "MobMonsterActionList.h"

class IMobStateMachine;

class MobMonster:public MonsterBase
{
public:

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="charaCon">移動させたいオブジェクトのキャラクターコントローラー</param>
	void MoveMonster(CharacterController& charaCon);

	/// <summary>
	/// パトロール時の移動処理
	/// </summary>
	/// <param name="charaCon"></param>
	void MovePatrol(CharacterController& charaCon);
	
	/// <summary>
	/// プレイヤーを追いかけるときの移動処理
	/// </summary>
	/// <param name="charaCon"></param>
	void MoveChasePlayer(CharacterController& charaCon);

	/// <summary>
	/// ノックバック処理
	/// </summary>
	void ProcessKnockBack(CharacterController& charaCon);

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
	virtual bool isAnimationEnable() const = 0;

	/// <summary>
	/// 回転可能か
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEnable() const = 0;

	/// <summary>
	/// 攻撃可能か
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEnable() const = 0;

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// やられたときの処理
	/// </summary>
	/// <param name="seFlag">やられたときの効果音を再生するかのフラグ</param>
	void ProcessDead(bool seFlag = true) override;

	/// <summary>
	/// ヒットエフェクト生成
	/// </summary>
	void CreateHitEffect();

	/// <summary>
	/// ノックバックフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetKnockBackFlag(bool flag)
	{
		m_knockBackFlag = flag;
	}
	/// <summary>
	/// ノックバックフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetKnockBackFlag() const
	{
		return m_knockBackFlag;
	}

	/// <summary>
	/// ノックバックによる硬直中か
	/// </summary>
	/// <returns>硬直中ならtrue</returns>
	const bool& IsKnockBackStiffness();

	///////////////////////////////////////////////////////////
	///取得、設定関数
	///////////////////////////////////////////////////////////

	/// <summary>
	/// プレイヤーが近くにいるかフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerNearbyFlag(bool flag)
	{
		m_playerNearbyFlag = flag;
	}
	/// <summary>
	/// プレイヤーが近くにいるかフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetPlayerNearbyFlag() const
	{
		return m_playerNearbyFlag;
	}


	/// <summary>
	/// 敵を見つけたか
	/// オーバーライドしなければ視野角判定で判断
	/// </summary>
	/// <returns>見つけたらtrue</returns>
	virtual bool IsFoundPlayerFlag();

	/// <summary>
	/// 攻撃範囲内にプレイヤーがいるか。
	/// </summary>
	/// <returns>いたらtrue</returns>
	virtual bool IsPlayerInAttackRange();
	
	/// <summary>
	/// 攻撃可能か。一定間隔で攻撃するため
	/// </summary>
	/// <returns></returns>
	bool IsProcessAttackEnable();
	
	/// <summary>
	/// スキル使用可能か
	/// </summary>
	/// <returns>使用可能ならtrue</returns>
	virtual bool IsSkillUsable();
	
	/// <summary>
	/// スキルのインターバルの計算
	/// </summary>
	void CalcSkillAttackIntarval();

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	virtual void SetNextAnimationState(MobMonsterInfo::EnAnimationState nextState){}

	/// <summary>
	/// 次のステートマシンを作成する
	/// </summary>
	/// <param name="nextStateMachine"></param>
	virtual void SetNextStateMachine(MobMonsterInfo::EnStateMachineState nextStateMachine){}

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


protected:

	IMobStateMachine* m_mobStateMachine = nullptr;
	
	Vector3					m_passPower = g_vec3One;		//はじくときの力

	Vector3					m_direction = Vector3::Zero;	//移動する方向ベクトル

	int					m_angleChangeTime = 0;		//向かうベクトルを計算するタイマー

	float					m_distanceToPlayer = 0.0f;		//プレイヤーとの距離
	float					m_attackRange = 0.0f;			//攻撃できる範囲
	float					m_skillAttackRange = 0.0f;		//スキル攻撃ができる範囲
	float					m_stayRange = 0.0f;				//移動しない範囲

	float					m_ramdomAngle = 0.0f;			//ランダムなアングル。移動方向を決めるときに使う

	float					m_attackIntervalTime = 0.0f;	//攻撃した後のインターバル

	int						m_angleRange = 2;				//移動するアングルの範囲

	float					m_pos2Length = 0.0;				//壁と判

	bool                    m_chasePlayerFlag = false;		//プレイヤーを追いかけるかのフラグ

	bool					m_knockBackFlag = false;		//ノックバックするかのフラグ

	float					m_knockBackTimer = 0.0f;		//ノックバックで移動する時間

	bool					m_playerNearbyFlag = false;

	float m_knockBackStiffnessTimer = 0.0f;


	//スキル使用可能かのタイマー
	float m_skillUsableLimmit = 5.0f;
	float m_skillUsableTimer = 0.0f;
	bool m_skillUsableFlag = true;


};

