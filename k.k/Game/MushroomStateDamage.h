#pragma once
#include "IMushroomState.h"

class MushroomStateDamage :public IMushroomState
{
public:
	MushroomStateDamage(Mushroom* mushroom)
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
