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
/// ノックバックの処理。
/// </summary>
/// <param name="knockBackPower"></param>
/// <param name="timer"></param>
/// <param name="moveSpeed"></param>
/// <returns>処理中はtrue、timerが上限に達するとfalse</returns>
bool IsProcessKnockBack(
	float &timer,
	Vector3& moveSpeed
);




