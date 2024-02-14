#pragma once

/// <summary>
/// ノックバックする時の方向
/// </summary>
/// <param name="startPosition">始点。攻撃の中心</param>
/// <param name="endPosition">終点。自身の座標</param>
/// <param name="knockBackPower">ノックバックパワー</param>
/// <returns></returns>
const Vector3 SetKnockBackDirection(
	Vector3 startPosition,
	Vector3 endPosition,
	float knockBackPower
);

/// <summary>
/// ノックバックの移動方向の処理。
/// </summary>
/// <param name="moveSpeed">移動方向</param>
/// <param name="timer">進行時間</param>
/// <param name="timerLimmit">ノックバックする制限時間</param>
/// <param name=""></param>
/// <returns>処理中はtrue、timerが上限に達するとfalse</returns>
bool IsKnockingBack(
	Vector3& moveSpeed,
	float &timer,
	const float timerLimmit=1.0f
);




