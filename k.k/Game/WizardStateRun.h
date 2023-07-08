#pragma once
#include "IWizardState.h"

class WizardStateRun:public IWizardState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WizardStateRun(Wizard* wizard)
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

