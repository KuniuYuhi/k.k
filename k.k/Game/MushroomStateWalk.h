#pragma once
#include "IMushroomState.h"

class MushroomStateWalk :public IMushroomState
{
public:
	MushroomStateWalk(Mushroom* mushroom)
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
