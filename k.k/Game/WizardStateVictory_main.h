#pragma once
#include "IWizardState.h"

class WizardStateVictory_main :public IWizardState
{
public:
	WizardStateVictory_main(Wizard* wizard)
		:IWizardState(wizard)
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

