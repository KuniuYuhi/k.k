#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateAttack_2 :public ITurtleShellState
{
public:
	TurtleShellStateAttack_2(TurtleShell* turtleShell)
		:ITurtleShellState(turtleShell)
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

