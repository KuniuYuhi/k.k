#pragma once
#include "ILichState.h"

class LichStateIdle:public ILichState
{
public:
	LichStateIdle(Lich* lich)
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

