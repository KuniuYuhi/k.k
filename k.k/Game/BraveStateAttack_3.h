#pragma once
#include "IBraveState.h"

class BraveStateAttack_3 :public IBraveState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="brave"></param>
	BraveStateAttack_3(Brave* brave)
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


