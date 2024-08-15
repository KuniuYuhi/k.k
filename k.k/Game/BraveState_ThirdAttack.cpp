#include "stdafx.h"
#include "BraveState_ThirdAttack.h"
#include "Brave.h"


void BraveState_ThirdAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_ThirdAttack + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_ThirdAttack::Entry()
{
	//�A�N�V�������ɂ���
	m_brave->ActionActive();
}

void BraveState_ThirdAttack::Ubdate()
{
	//�A�j���[�V�������I�������
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�̋��ʏ���
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveState_ThirdAttack::Exit()
{
	//�A�N�V�������I���
	m_brave->ExitAttackAction();
}
