#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateWalk :public ITurtleShellState
{
public:
	TurtleShellStateWalk(TurtleShell* turtleShell)
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
