#include "stdafx.h"
#include "WizardStateWalk.h"
#include "Wizard.h"
//�����X�e�[�g�N���X
void WizardStateWalk::ManageState()
{
	m_wizard->ProcessCommonStateTransition();
}

void WizardStateWalk::PlayAnimation()
{
	//�����A�j���[�V�����̍Đ�
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Walk,0.3f);
}
