#pragma once
#include "IWizardState.h"

class WizardState_Attack_4:public IWizardState
{
public:
	WizardState_Attack_4(Wizard* wizard)
		:IWizardState(wizard)
	{
	}

	/// <summary>
	/// �E�B�U�[�h�̏�ԑJ�ڊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
};

