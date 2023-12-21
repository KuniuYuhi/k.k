#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateChase :public ITurtleShellState
{
public:
	TurtleShellStateChase(TurtleShell* turtleShell)
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
