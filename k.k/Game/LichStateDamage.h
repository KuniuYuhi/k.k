#pragma once
#include "ILichState.h"

class LichStateDamage:public ILichState
{
public:
	LichStateDamage(Lich* lich)
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

