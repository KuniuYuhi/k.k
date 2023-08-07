#pragma once
#include "ILichState.h"

class LichStateSummon:public ILichState
{
public:
	LichStateSummon(Lich* lich)
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

