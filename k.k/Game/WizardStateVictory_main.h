#pragma once
#include "IWizardState.h"

class WizardStateVictory_main :public IWizardState
{
public:
	WizardStateVictory_main(Wizard* wizard)
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

