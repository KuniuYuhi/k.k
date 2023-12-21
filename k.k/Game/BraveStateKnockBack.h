#pragma once
#include "IBraveState.h"

class BraveStateKnockBack :public IBraveState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="brave"></param>
	BraveStateKnockBack(Brave* brave)
		:IBraveState(brave)
	{
	}

	/// <summary>
	/// 勇者の状態遷移管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();

private:

	/// <summary>
	/// 硬直しているか。タイマーで計算
	/// </summary>
	/// <returns></returns>
	bool IsStiffness();

private:
	float m_stiffnessTimer = 0.0f;
	bool m_knockBackEndFlag = false;
	
};
