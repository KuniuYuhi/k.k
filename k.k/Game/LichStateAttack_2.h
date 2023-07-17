#pragma once
#include "ILichState.h"

class LichStateAttack_2 :public ILichState
{
public:
	LichStateAttack_2(Lich* lich)
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

