#pragma once
#include "IBraveState.h"

class BraveStateDie :public IBraveState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="brave"></param>
	BraveStateDie(Brave* brave)
		:IBraveState(brave)
	{
	}

	/// <summary>
	/// �E�҂̏�ԑJ�ڊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
};
