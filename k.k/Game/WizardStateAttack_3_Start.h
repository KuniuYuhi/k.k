#pragma once
#include "IWizardState.h"

class WizardStateAttack_3_Start:public IWizardState
{
public:
	WizardStateAttack_3_Start(Wizard* wizard)
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

