#pragma once

/// <summary>
/// 目玉のステート
/// </summary>
namespace BeholderEyeStates {

	/// <summary>
	/// 目玉のステート
	/// </summary>
	enum EnBeholderEyeState
	{
		enBeholderEyeState_Idle,				//待機
		enBeholderEyeState_Run,					//歩き
		enBeholderEyeState_Attack,				//攻撃
		enBeholderEyeState_Hit,					//被ダメージ
		enBeholderEyeState_Die,					//死亡
		enBeholderEyeState_Victory,				//勝利
		enBeholderEyeState_Appear,				//出現
	};

}

/// <summary>
/// 目玉のアニメーションクリップ
/// </summary>
namespace BeholderEyeAnimationClips {

	enum EnBeholderEyeAnimClip
	{
		enBeholderEyeAnimClip_Idle,					//待機
		enBeholderEyeAnimClip_Run,					//歩き
		enBeholderEyeAnimClip_Attack,				//攻撃
		enBeholderEyeAnimClip_Hit,					//被ダメージ
		enBeholderEyeAnimClip_Die,					//死亡
		enBeholderEyeAnimClip_Victory,				//勝利
		enBeholderEyeAnimClip_Appear,				//出現
		enBeholderEyeAnimClip_num
	};


}