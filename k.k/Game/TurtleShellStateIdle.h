#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateIdle:public ITurtleShellState
{
public:
	TurtleShellStateIdle(TurtleShell* turtleShell)
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

