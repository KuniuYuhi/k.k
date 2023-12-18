#pragma once
#include "MobMonster.h"

using namespace MobMonsterInfo;

class Lich;
class ISlimeState;
class IMobStateMachine;

class Slime :public MobMonster
{
public:
	Slime();
	~Slime();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// 処理を止めるか
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	/// <summary>
	/// 当たり判定生成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// モデルレンダーの取得
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isAnimationEnable() const override
	{
		return m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEnable() const override
	{
		return m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	/// 攻撃中なら
	/// </summary>
	/// <returns></returns>
	bool IsAttackEnable() const override
	{
		return m_enAnimationState != enAnimationState_Attack &&
			m_enAnimationState != enAnimationState_Skill;
	}


	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	void Damage(int attack);

	/// <summary>
	/// 共通のステート遷移処理を実行
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// アタック１ステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_1StateTransition();
	/// <summary>
	/// 被ダメージステート遷移処理を実行
	/// </summary>
	void OnProcessDamageStateTransition();
	/// <summary>
	/// デスステート遷移処理を実行
	/// </summary>
	void OnProcessDieStateTransition();
	/// <summary>
	/// 勝利ステート遷移処理を実行
	/// </summary>
	void OnProcessVictoryStateTransition();
	/// <summary>
	/// 召喚された時のステート遷移処理を実行
	/// </summary>
	void OnProcessAppearStateTransition();

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState) override;

	/// <summary>
	/// キャラクターコントローラーの取得
	/// </summary>
	/// <returns></returns>
	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

	/// <summary>
	/// 当たり判定生成フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetCreateAttackCollisionFlag() const
	{
		return m_createAttackCollisionFlag;
	}

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel();

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();

	/// <summary>
	/// 攻撃時の音を再生
	/// </summary>
	void PlayAttackSound();


	IMobStateMachine* m_stateMachine = nullptr;
	
	ISlimeState* m_state = nullptr;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimationClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	RigidBody m_rigidBody;		//剛体。

	CollisionObject* m_headCollision = nullptr;

	ModelRender m_modelRender;
	CharacterController m_charaCon;

	int m_attackBoonId = -1;					//攻撃で使うボーンID

	bool m_createAttackCollisionFlag = false;

};

