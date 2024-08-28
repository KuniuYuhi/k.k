#pragma once

/// <summary>
/// スライムのステート
/// </summary>
namespace SlimeStates {

	/// <summary>
	/// スライムのステート
	/// </summary>
	enum EnSlimeState
	{
		enSlimeState_Idle,					//待機
		enSlimeState_Run,					//歩き
		enSlimeState_Attack,				//攻撃
		enSlimeState_Hit,					//被ダメージ
		enSlimeState_Die,					//死亡
		enSlimeState_Victory,				//勝利
		enSlimeState_Appear,				//出現
	};

}

/// <summary>
/// スライムのアニメーションクリップ
/// </summary>
namespace SlimeAnimationClips {

	enum EnSlimeAnimClip
	{
		enSlimeAnimClip_Idle,					//待機
		enSlimeAnimClip_Run,					//歩き
		enSlimeAnimClip_Attack,				//攻撃
		enSlimeAnimClip_Hit,					//被ダメージ
		enSlimeAnimClip_Die,					//死亡
		enSlimeAnimClip_Victory,				//勝利
		enSlimeAnimClip_Appear,				//出現
		enSlimeAnimClip_num
	};


}