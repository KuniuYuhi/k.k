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
	/// 状態を管理する
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
};

