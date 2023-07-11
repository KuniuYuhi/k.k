#pragma once
#include "Actor.h"

class IWizardState;
class Wizard :public Actor
{
public:
	Wizard();
	~Wizard();

	bool Start();

	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel();

	void Update();
	void Render(RenderContext& rc);

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Move();

	void Attack();

	void PlayAnimation();

	void ManageState();

	/// <summary>
	/// 特定のアニメーションが再生中(true)なら入れ替え可能。
	/// </summary>
	/// <returns></returns>
	bool isAnimationSwappable() const
	{
		return m_enAnimationState != enAninationState_Idle &&
			m_enAnimationState != enAninationState_Walk &&
			m_enAnimationState != enAninationState_Run;
	}

	/// <summary>
	/// 特定のアニメーションが再生中なら移動できない
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2_start &&
			m_enAnimationState != enAnimationState_Attack_2_main &&
			m_enAnimationState != enAnimationState_Attack_3_start &&
			m_enAnimationState != enAnimationState_Attack_3_main &&
			m_enAnimationState != enAnimationState_Attack_4;
	}

	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// 共通のステート遷移処理を実行
	/// </summary>
	void ProcessCommonStateTransition();

	void OnProcessAttack_1StateTransition();

	void OnProcessAttack_2StateTransition();

	void OnProcessAttack_3StateTransition();

	void OnProcessAttack_4StateTransition();


	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : 待機アニメーション
		enAnimClip_Walk,			// 1 : 歩きアニメーション
		enAnimClip_Run,				// 2 : 走りアニメーション
		enAnimClip_Attack_1,		// 3 : 
		enAnimClip_Attack_2_start,		// 4 : 
		enAnimClip_Attack_2_main,
		enAnimClip_Attack_3_start,		// 5 : 
		enAnimClip_Attack_3_main,
		enAnimClip_Attack_4,		// 6 : 
		enAnimClip_Num,				// 7 :アニメーションクリップの数
	};

	

private:
	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2_start,
		enAnimationState_Attack_2_main,
		enAnimationState_Attack_3_start,
		enAnimationState_Attack_3_main,
		enAnimationState_Attack_4
	};

	//攻撃パターンステート
	enum EnAttackPattern {
		enAttackPattern_None,
		enAttackPattern_1,
		enAttackPattern_2_start,
		enAttackPattern_2_main,
		enAttackPattern_3_start,
		enAttackPattern_3_main,
		enAttackPattern_4,
		enAttackPattern_End
	};

public:
	//アニメーションステートを設定する
	void SetAnimationState()
	{
		//m_enAnimationState= enAninationState_Idle;
	}

private:
	
	
	void SetNextAnimationState(EnAnimationState nextState);


	

	ModelRender m_modelRender;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	EnAttackPattern m_enAttackPatternState = enAttackPattern_None;

	IWizardState* m_animationState = nullptr;
};

