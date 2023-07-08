#pragma once
#include "IWizardState.h"

class WizardStateWalk:public IWizardState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WizardStateWalk(Wizard* wizard)
		//ウィザードのインスタンスを代入する
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

