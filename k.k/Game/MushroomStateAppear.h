#pragma once
#include "IMushroomState.h"

class MashroomStateAppear :public IMushroomState
{
public:
	MashroomStateAppear(Mushroom* mushroom)
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
