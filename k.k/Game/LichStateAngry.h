#pragma once
#include "ILichState.h"

class LichStateAngry :public ILichState
{
public:
	LichStateAngry(Lich* lich)
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
