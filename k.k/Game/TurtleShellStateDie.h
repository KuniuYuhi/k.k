#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateDie :public ITurtleShellState
{
public:
	TurtleShellStateDie(TurtleShell* turtleShell)
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

