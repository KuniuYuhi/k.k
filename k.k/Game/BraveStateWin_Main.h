#pragma once
#include "IBraveState.h"

class BraveStateWin_Main :public IBraveState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="brave"></param>
	BraveStateWin_Main(Brave* brave)
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


