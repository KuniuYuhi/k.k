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
	/// 状態を管理する
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
};
