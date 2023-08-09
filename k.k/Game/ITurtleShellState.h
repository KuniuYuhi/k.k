#pragma once

class TurtleShell;
/// <summary>
/// とげ甲羅ステートの基底クラス
/// </summary>
class ITurtleShellState
{
public:
	ITurtleShellState(TurtleShell* turtleShell)
	{
		m_turtleShell = turtleShell;
	}

	virtual ~ITurtleShellState() {}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;


protected:
	TurtleShell* m_turtleShell = nullptr;

};

