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
	/// 状態を管理する
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();

private:

	bool m_animationResetFlag = true;
};
