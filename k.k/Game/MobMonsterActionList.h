#pragma once

namespace MobMonsterInfo {

	/// <summary>
	/// アクションの名前
	/// </summary>
	enum EnActionName
	{
		enActionName_Idle,		//待機状態
		enActionName_Patrol,	//巡回
		enActionName_Chase,		//追いかける
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
		enAninationState_Patrol,	//巡回
		enAninationState_Chase,		//追いかける
		enAnimationState_Attack,	//攻撃
		enAnimationState_Skill,		//スキル
		enAnimationState_Hit,		//被ダメージ
		enAnimationState_Die,		//死亡
		enAnimationState_Victory,	//勝利
		enAnimationState_Appear,	//出現時のアニメーション
		enAnimationState_Num
	};

	/// <summary>
	/// アニメーションクリップ
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,		//待機状態
		enAnimationClip_Patrol,		//巡回
		enAnimationClip_Chase,		//追いかける
		enAnimationClip_Attack,		//攻撃
		enAnimationClip_Skill,		//スキル
		enAnimationClip_Hit,		//被ダメージ
		enAnimationClip_Die,		//死亡
		enAnimationClip_Victory,	//勝利
		enAnimationClip_Appear,		//出現時のアニメーション
		enAnimationClip_Num
	};

	/// <summary>
	/// ステートマシンステート
	/// </summary>
	enum EnStateMachineState
	{
		enStateMachineState_Patrol,		//巡回ステートマシン
		enStateMachineState_Chase		//追跡ステートマシン
	};
}





