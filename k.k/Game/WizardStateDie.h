#pragma once
#include "IWizardState.h"

class WizardStateDie :public IWizardState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WizardStateDie(Wizard* wizard)
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

