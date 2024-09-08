#pragma once

/// <summary>
/// キノコのステート
/// </summary>
namespace MushroomStates {

	/// <summary>
	/// キノコのステート
	/// </summary>
	enum EnMushroomState
	{
		enMushroomState_Idle,					//待機
		enMushroomState_Run,					//歩き
		enMushroomState_Attack,				//攻撃
		enMushroomState_Hit,					//被ダメージ
		enMushroomState_Die,					//死亡
		enMushroomState_Victory,				//勝利
		enMushroomState_Appear,				//出現
	};

}

/// <summary>
/// キノコのアニメーションクリップ
/// </summary>
namespace MushroomAnimationClips {

	enum EnMushroomAnimClip
	{
		enMushroomAnimClip_Idle,					//待機
		enMushroomAnimClip_Run,					//歩き
		enMushroomAnimClip_Attack,				//攻撃
		enMushroomAnimClip_Hit,					//被ダメージ
		enMushroomAnimClip_Die,					//死亡
		enMushroomAnimClip_Victory,				//勝利
		enMushroomAnimClip_Appear,				//出現
		enMushroomAnimClip_num
	};


}