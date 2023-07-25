#pragma once
#include "IWizardState.h"

class WizardStateDamage:public IWizardState
{
public:
	WizardStateDamage(Wizard* wizard)
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

