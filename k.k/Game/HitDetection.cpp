#include "stdafx.h"
#include "HitDetection.h"

bool HitDetection::IsHittable(const float hittableTime)
{
	//既にヒット可能なら処理しない
	if (hittableFlag == true)
	{
		return true;
	}
	//ヒット可能な制限時間に達したら
	if (timer >= hittableTime)
	{
		//タイマーをリセット
		timer = 0.0f;
		hittableFlag = true;
		return true;
	}
	else
	{
		//タイマーを加算
		timer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}
