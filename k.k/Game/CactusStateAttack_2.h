#pragma once
#include "ICactusState.h"

class CactusStateAttack_2 :public ICactusState
{
public:
	CactusStateAttack_2(Cactus* cactus)
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
