#include "stdafx.h"
#include "HitDetection.h"

bool HitDetection::IsHittable(const float hittableTime)
{
	//���Ƀq�b�g�\�Ȃ珈�����Ȃ�
	if (hittableFlag == true)
	{
		return true;
	}
	//�q�b�g�\�Ȑ������ԂɒB������
	if (timer >= hittableTime)
	{
		//�^�C�}�[�����Z�b�g
		timer = 0.0f;
		hittableFlag = true;
		return true;
	}
	else
	{
		//�^�C�}�[�����Z
		timer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}
