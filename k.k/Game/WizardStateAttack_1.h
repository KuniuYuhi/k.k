#pragma once
#include "IWizardState.h"

class WizardStateAttack_1:public IWizardState
{
public:
	WizardStateAttack_1(Wizard* wizard)
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

