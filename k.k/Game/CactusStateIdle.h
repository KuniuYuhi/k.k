#pragma once
#include "ICactusState.h"

class CactusStateIdle :public ICactusState
{
public:
	CactusStateIdle(Cactus* cactus)
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
