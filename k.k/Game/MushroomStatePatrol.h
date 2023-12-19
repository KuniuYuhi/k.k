#pragma once
#include "IMushroomState.h"

class MushroomStatePatrol :public IMushroomState
{
public:
	MushroomStatePatrol(Mushroom* mushroom)
		:IMushroomState(mushroom)
	{
	}

	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
};