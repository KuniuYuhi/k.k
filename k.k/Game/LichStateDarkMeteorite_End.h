#pragma once
#include "ILichState.h"

class LichStateDarkMeteorite_End:public ILichState
{
public:
	LichStateDarkMeteorite_End(Lich* lich)
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

