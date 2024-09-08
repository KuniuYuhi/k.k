#include "stdafx.h"
#include "CactusState_Appear.h"
#include "Cactus.h"

CactusState_Appear::~CactusState_Appear()
{
}

void CactusState_Appear::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Appear, 0.2f
	);
}

void CactusState_Appear::Entry()
{
	//�A�N�V�������ɂ���
	m_cactus->ActionActive();

	//�v���C���[�̕��������悤�ɂ���
	m_cactus->TurnToPlayer();
}

void CactusState_Appear::Ubdate()
{

	//�A�j���[�V�������I�������
	if (m_cactus->GetModelRender().IsPlayingAnimation() == false)
	{
		m_cactus->ProcessCommonTranstion();
	}
}

void CactusState_Appear::Exit()
{
	//�A�N�V�������I���
	m_cactus->ActionDeactive();
}
