#pragma once
#include "ICactusState.h"

class CactusStatePlantToBattle :public ICactusState
{
public:
	CactusStatePlantToBattle(Cactus* cactus)
		:ICactusState(cactus)
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
