#pragma once
#include "IMushroomState.h"

class MushroomStateVictory :public IMushroomState
{
public:
	MushroomStateVictory(Mushroom* mushroom)
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
