#include "stdafx.h"
#include "WizardStateIdle.h"
#include "Wizard.h"
//�ҋ@�X�e�[�g�N���X
void WizardStateIdle::ManageState()
{
	m_wizard->ProcessCommonStateTransition();
}

void WizardStateIdle::PlayAnimation()
{
	//�ҋ@�A�j���[�V�����̍Đ�
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Idle);
}
