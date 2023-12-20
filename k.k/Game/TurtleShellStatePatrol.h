#pragma once
#include "ITurtleShellState.h"

class TurtleShellStatePatrol :public ITurtleShellState
{
public:
	TurtleShellStatePatrol(TurtleShell* turtleShell)
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
