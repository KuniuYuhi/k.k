#pragma once
#include "IWizardState.h"

class WizardStateAttack_3_Start:public IWizardState
{
public:
	WizardStateAttack_3_Start(Wizard* wizard)
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

