#pragma once

namespace KnockBackInfo {

	/// <summary>
	/// ノックバックのパターン
	/// </summary>
	enum EnKnockBackPattern
	{
		ShortGroundedRetreat,  // 少し後ずさる
		LongGroundedRetreat,   // 長めに後ずさる
		SlightAirborneRetreat, // 少し吹っ飛びながら後ずさる
		FullAirborneRetreat,   // 強めに吹っ飛びながら後ずさる
		VerticalLaunch,        // その場から動かずに空中に吹っ飛ぶ(空中に浮く)

		enPattern_Num,			//総数

	};

}
