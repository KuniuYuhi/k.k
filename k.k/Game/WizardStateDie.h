#pragma once
#include "IWizardState.h"

class WizardStateDie :public IWizardState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WizardStateDie(Wizard* wizard)
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

