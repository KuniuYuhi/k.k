#pragma once
#include "ILichState.h"

class LichStateDie :public ILichState
{
public:
	LichStateDie(Lich* lich)
		:ILichState(lich)
	{
	}

	/// <summary>
	/// ���b�`�̏�ԑJ�ڊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
};

