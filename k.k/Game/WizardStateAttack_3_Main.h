#pragma once
#include "IWizardState.h"

class WizardStateAttack_3_Main:public IWizardState
{
public:
	WizardStateAttack_3_Main(Wizard* wizard)
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

