#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateVictory :public ITurtleShellState
{
public:
	TurtleShellStateVictory(TurtleShell* turtleShell)
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

