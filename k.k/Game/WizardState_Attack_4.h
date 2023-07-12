#pragma once
#include "IWizardState.h"

class WizardState_Attack_4:public IWizardState
{
public:
	WizardState_Attack_4(Wizard* wizard)
		:IWizardState(wizard)
	{
	}

	/// <summary>
	/// ウィザードの状態遷移管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
};

