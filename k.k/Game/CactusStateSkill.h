#pragma once
#include "ICactusState.h"

class CactusStateSkill :public ICactusState
{
public:
	CactusStateSkill(Cactus* cactus)
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