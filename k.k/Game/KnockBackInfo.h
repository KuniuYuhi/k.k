#pragma once

/// <summary>
/// ノックバックパターン
/// </summary>
namespace KnockBackInfo {

	/// <summary>
	/// ノックバックのパターン
	/// </summary>
	enum EnKnockBackPattern
	{
		enKBPattern_ShortGroundedRetreat,  // 少し後ずさる
		enKBPattern_LongGroundedRetreat,   // 長めに後ずさる
		enKBPattern_SlightAirborneRetreat, // 少し吹っ飛びながら後ずさる
		enKBPattern_FullAirborneRetreat,   // 強めに吹っ飛びながら後ずさる
		enKBPattern_VerticalLaunch,        // その場から動かずに空中に吹っ飛ぶ(空中に浮く)

		enKnockBackPattern_Num,			//総数

	};


	/// <summary>
	/// カーブデータのポイント
	/// </summary>
	struct CurvePoint {
		float time = 0.0f;
		Vector3 curvePosition = g_vec3Zero;
	};

}
