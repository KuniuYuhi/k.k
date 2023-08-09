#pragma once
#include "ITurtleShellState.h"

class TurtleShellStateAttack_2 :public ITurtleShellState
{
public:
	TurtleShellStateAttack_2(TurtleShell* turtleShell)
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
};

