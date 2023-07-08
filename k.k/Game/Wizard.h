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

	void Move();

	void PlayAnimation();

	void ManageState();

	/// <summary>
	/// 特定のアニメーションが再生中(true)なら入れ替え可能。
	/// </summary>
	/// <returns></returns>
	bool isAnimationSwappable() const
	{
		return m_enAnimationState == enAninationState_Idle &&
			m_enAnimationState == enAninationState_Walk &&
			m_enAnimationState == enAninationState_Run;
	}

	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// 共通のステート遷移処理を実行
	/// </summary>
	void ProcessCommonStateTransition();

	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : 待機アニメーション
		enAnimClip_Walk,	// 1 : 歩きアニメーション
		enAnimClip_Run,		// 2 : 走りアニメーション
		enAnimClip_Num,		// 3 :アニメーションクリップの数
	};

	

private:
	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run
	};

public:
	//アニメーションステートを設定する
	void SetAnimationState()
	{
		m_enAnimationState= enAninationState_Idle;
	}

private:
	
	void OnProcessRunStateTransition();
	void SetNextAnimationState(EnAnimationState nextState);


	

	ModelRender m_modelRender;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	IWizardState* m_animationState = nullptr;
};

