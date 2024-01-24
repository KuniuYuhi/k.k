#pragma once

namespace {
	const int LENGTH = 800.0f;
}

//メテオの終点の計算

/// <summary>
/// メテオの終点の取得。プレイヤーの周りに落とす
/// </summary>
/// <param name="targetCenterPosition">メテオを落とす中心の座標</param>
/// <returns></returns>
Vector3 GetMeteoTargetPosition(Vector3 targetCenterPosition)
{
	while (true)
	{
		Vector3 finalPos = targetCenterPosition;
		//中心から300の範囲内でランダムな値を決める
		float X = rand() % (LENGTH * 2 + 1) - (LENGTH);
		float Z = rand() % (LENGTH * 2 + 1) - (LENGTH);

		finalPos.x += X;
		finalPos.z += Z;
		finalPos.y = 0.0f;
		return finalPos;
	}
}