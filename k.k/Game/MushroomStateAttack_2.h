#pragma once
#include "IMushroomState.h"

class MushroomStateAttack_2 :public IMushroomState
{
public:
	MushroomStateAttack_2(Mushroom* mushroom)
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
