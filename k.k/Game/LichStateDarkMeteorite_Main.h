#pragma once
#include "ILichState.h"

class LichStateDarkMeteorite_Main:public ILichState
{
public:
	LichStateDarkMeteorite_Main(Lich* lich)
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

