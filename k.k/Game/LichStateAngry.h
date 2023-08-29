#pragma once
#include "ILichState.h"

class LichStateAngry :public ILichState
{
public:
	LichStateAngry(Lich* lich)
		:ILichState(lich)
	{
	}

	/// <summary>
	/// リッチの状態遷移管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
};
