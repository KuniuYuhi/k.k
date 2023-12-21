#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateSkill :public ITurtleShellState
{
public:
	TurtleShellStateSkill(TurtleShell* turtleShell)
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

