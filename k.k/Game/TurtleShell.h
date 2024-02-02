#pragma once

#include "MobMonster.h"

using namespace MobMonsterInfo;

class ITurtleShellState;
class IMobStateMachine;

class TurtleShell:public MobMonster
{
public:
	TurtleShell();
	~TurtleShell();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// 当たり判定生成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// 処理を止めるか
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	/// <summary>
	///  防御を終わるか
	/// </summary>
	/// <returns>trueで終わる</returns>
	bool IsDifenceEnd();

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
		return //m_enAnimationState != enAnimationState_Difence &&
			//m_enAnimationState != enAnimationState_DifenceDamage &&
			m_enAnimationState != enAnimationState_Hit &&
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
	/// 攻撃中か
	/// </summary>
	/// <returns></returns>
	bool IsAttackEnable() const override
	{
		return /*m_enAnimationState != enAnimationState_Difence &&*/
			m_enAnimationState != enAnimationState_Attack &&
			m_enAnimationState != enAnimationState_Skill;
	}

	//通常攻撃に当たった時の処理
	void HitNormalAttack() override;

	//スキルに当たった時の処理
	void HitSkillAttack() override;

	/// <summary>
	/// スキル使用可能かタイマー
	/// </summary>
	/// <returns>使用可能ならtrue</returns>
	bool IsSkillUsable() override;

	/*/// <summary>
	/// 敵を見つけたか
	/// </summary>
	/// <returns>見つけたらtrue</returns>
	bool IsFoundPlayerFlag() override;*/

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	void Damage(int attack);

	bool RotationOnly();

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
	void SetNextAnimationState(EnAnimationState nextState);

	/// <summary>
	/// 次のステートマシンを作成する
	/// </summary>
	/// <param name="nextStateMachine"></param>
	void SetNextStateMachine(EnStateMachineState nextStateMachine) override;

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
	
	ITurtleShellState* m_state = nullptr;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimationClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	EnStateMachineState m_enStateMachineState = enStateMachineState_Patrol;

	CollisionObject* m_headCollision = nullptr;
	
	ModelRender m_modelRender;

	
	int m_attackBoonId = -1;					//攻撃で使うボーンID

	bool m_createAttackCollisionFlag = false;


	bool m_damagedFlag = false;			//自身が攻撃を受けたかのフラグ
	bool m_difenceEnableFlag = false;			//防御しているかのフラグ
	const float m_difenceTime = 3.0f;
	float m_difenceTimer = 0.0f;

};

