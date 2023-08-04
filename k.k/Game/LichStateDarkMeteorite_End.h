#pragma once
#include "ILichState.h"

class LichStateDarkMeteorite_End:public ILichState
{
public:
	LichStateDarkMeteorite_End(Lich* lich)
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

