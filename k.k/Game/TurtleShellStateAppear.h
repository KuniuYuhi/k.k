#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateAppear :public ITurtleShellState
{
public:
	TurtleShellStateAppear(TurtleShell* turtleShell)
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

