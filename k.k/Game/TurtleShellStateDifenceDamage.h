#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateDifenceDamage :public ITurtleShellState
{
public:
	TurtleShellStateDifenceDamage(TurtleShell* turtleShell)
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

