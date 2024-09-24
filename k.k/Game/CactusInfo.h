#pragma once

/// <summary>
/// サボテンのステート
/// </summary>
namespace CactusStates {

	/// <summary>
	///  サボテンのステート
	/// </summary>
	enum EnCactusState
	{
		enCactusState_Idle,					//待機
		enCactusState_Run,					//歩き
		enCactusState_Attack,				//攻撃
		enCactusState_Hit,					//被ダメージ
		enCactusState_Die,					//死亡
		enCactusState_Victory,				//勝利
		enCactusState_Appear,				//出現
	};

}

/// <summary>
///  サボテンのアニメーションクリップ
/// </summary>
namespace CactusAnimationClips {

	enum EnCactusAnimClip
	{
		enCactusAnimClip_Idle,					//待機
		enCactusAnimClip_Run,					//歩き
		enCactusAnimClip_Attack,				//攻撃
		enCactusAnimClip_Hit,					//被ダメージ
		enCactusAnimClip_Die,					//死亡
		enCactusAnimClip_Victory,				//勝利
		enCactusAnimClip_Appear,				//出現
		enCactusAnimClip_num
	};


}