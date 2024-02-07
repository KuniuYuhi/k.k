#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateDamage :public ITurtleShellState
{
public:
	TurtleShellStateDamage(TurtleShell* turtleShell)
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

private:

	bool m_animationResetFlag = true;
};
