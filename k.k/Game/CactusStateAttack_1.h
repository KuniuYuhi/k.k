#pragma once
#include "ICactusState.h"

class CactusStateAttack_1 :public ICactusState
{
public:
	CactusStateAttack_1(Cactus* cactus)
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
