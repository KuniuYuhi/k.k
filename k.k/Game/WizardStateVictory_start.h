#pragma once
#include "IWizardState.h"

class WizardStateVictory_start :public IWizardState
{
public:
	WizardStateVictory_start(Wizard* wizard)
		:IWizardState(wizard)
	{
	}

	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
};
