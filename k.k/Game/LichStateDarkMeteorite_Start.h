#pragma once
#include "ILichState.h"

class LichStateDarkMeteorite_Start:public ILichState
{
public:
	LichStateDarkMeteorite_Start(Lich* lich)
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

