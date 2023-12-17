#pragma once


/// <summary>
/// アクションの名前
/// </summary>
enum EnActionName
{
	enActionName_Idle,		//待機状態
	enActionName_Walk,		//歩き
	enActionName_Attack,	//攻撃
	enActionName_Skill,		//スキル
	enActionName_Hit,		//被ダメージ
	enActionName_Die,		//死亡
	enActionName_Victory,	//勝利
	enActionName_Appear		//出現時のアニメーション
};

/// <summary>
/// アニメーションステート
/// </summary>
enum EnAnimationState
{
	enAninationState_Idle,		//待機状態
	enAninationState_Walk,		//歩き
	enAnimationState_Attack,	//攻撃
	enAnimationState_Skill,		//スキル
	enAnimationState_Hit,		//被ダメージ
	enAnimationState_Die,		//死亡
	enAnimationState_Victory,	//勝利
	enAnimationState_Appear		//出現時のアニメーション
};

/// <summary>
/// アニメーションクリップ
/// </summary>
enum EnAnimationClip
{
	enAninationClip_Idle,		//待機状態
	enAninationClip_Walk,		//歩き
	enAnimationClip_Attack,		//攻撃
	enAnimationClip_Skill,		//スキル
	enAnimationClip_Hit,		//被ダメージ
	enAnimationClip_Die,		//死亡
	enAnimationClip_Victory,	//勝利
	enAnimationClip_Appear		//出現時のアニメーション
};




