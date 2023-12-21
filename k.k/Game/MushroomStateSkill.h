#pragma once
#include "IMushroomState.h"

class MushroomStateSkill :public IMushroomState
{
public:
	MushroomStateSkill(Mushroom* mushroom)
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
