#pragma once
#include "IWizardState.h"

class WizardStateAttack_1:public IWizardState
{
public:
	WizardStateAttack_1(Wizard* wizard)
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

