#pragma once
#include "ICactusState.h"

class CactusStateChase :public ICactusState
{
public:
	CactusStateChase(Cactus* cactus)
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
