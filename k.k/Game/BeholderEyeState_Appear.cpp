#include "stdafx.h"
#include "BeholderEyeState_Appear.h"
#include "BeholderEye.h"

BeholderEyeState_Appear::~BeholderEyeState_Appear()
{
}

void BeholderEyeState_Appear::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(
		enBeholderEyeAnimClip_Appear, 0.2f
	);
}

void BeholderEyeState_Appear::Entry()
{
	//�A�N�V�������ɂ���
	m_beholderEye->ActionActive();

	//�v���C���[�̕��������悤�ɂ���
	m_beholderEye->TurnToPlayer();
}

void BeholderEyeState_Appear::Ubdate()
{
	//�A�j���[�V�������I�������
	if (m_beholderEye->GetModelRender().IsPlayingAnimation() == false)
	{
		m_beholderEye->ProcessCommonTranstion();
	}
}

void BeholderEyeState_Appear::Exit()
{
	//�A�N�V�������I���
	m_beholderEye->ActionDeactive();
}
