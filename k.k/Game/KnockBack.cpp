#include "stdafx.h"
#include "KnockBack.h"

const Vector3 SetKnockBackDirection(
	Vector3 startPosition, Vector3 endPosition,float knockBackPower)
{
	//始点から終点に向かうベクトルを計算
	Vector3 finalPos;
	finalPos = startPosition - endPosition;



	//正規化
	finalPos.y = 0.0f;
	finalPos.Normalize();
	//ノックバックパワーを掛ける
	//パワーが大きいほど吹っ飛ぶ
	finalPos *= knockBackPower;

	return finalPos;
}

bool IsKnockingBack(
	Vector3& moveSpeed,
	float& timer, 
	const float timerLimmit
)
{
	if (timer >= timerLimmit)
	{
		//タイマーをリセット
		timer = 0.0f;
		//キャラの向いている方向とベクトルが逆なので
		//-1を掛けて戻す
		moveSpeed *= -1.0f;
		return false;
	}

	//タイマーを加算
	timer += g_gameTime->GetFrameDeltaTime()*2.0f;

	return true;
}
