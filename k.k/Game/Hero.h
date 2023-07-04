#pragma once
#include "Actor.h"

class Hero :public Actor
{
public:
	Hero();
	~Hero();

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
		return m_enAnimationState == enIdle &&
			m_enAnimationState == enWalk &&
			m_enAnimationState == enRun;
	}

private:
	//アニメーションステート
	enum EnAnimationState {
		enIdle,
		enWalk,
		enRun
	};

	void OnProcessCommonStateTransition();
	void OnProcessRunStateTransition();



	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : 待機アニメーション
		enAnimClip_Walk,	// 1 : 歩きアニメーション
		enAnimClip_Run,		// 2 : 走りアニメーション
		enAnimClip_Num,		// 3 :アニメーションクリップの数
	};

	ModelRender m_modelRender;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enIdle;	//アニメーションステート
	
	
	

};

