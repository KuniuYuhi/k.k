#pragma once
#include "IWizardState.h"

class WizardStateAttack_2_main:public IWizardState
{
public:
	WizardStateAttack_2_main(Wizard* wizard)
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

