#pragma once
#include "IWizardState.h"

class WizardStateAttack_2_main:public IWizardState
{
public:
	WizardStateAttack_2_main(Wizard* wizard)
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

