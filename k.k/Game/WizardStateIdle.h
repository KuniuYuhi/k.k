#pragma once
#include "IWizardState.h"

class WizardStateIdle:public IWizardState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WizardStateIdle(Wizard* wizard)
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

