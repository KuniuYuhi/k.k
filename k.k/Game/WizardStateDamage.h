#pragma once
#include "IWizardState.h"

class WizardStateDamage:public IWizardState
{
public:
	WizardStateDamage(Wizard* wizard)
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

