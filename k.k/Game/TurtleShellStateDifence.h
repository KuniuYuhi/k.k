#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateDifence :public ITurtleShellState
{
public:
	TurtleShellStateDifence(TurtleShell* turtleShell)
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

