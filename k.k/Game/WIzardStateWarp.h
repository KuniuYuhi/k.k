#pragma once
#include "IWizardState.h"

class WIzardStateWarp :public IWizardState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WIzardStateWarp(Wizard* wizard)
		//�E�B�U�[�h�̃C���X�^���X��������
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

