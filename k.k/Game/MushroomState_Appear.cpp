#include "stdafx.h"
#include "MushroomState_Appear.h"
#include "Mushroom.h"

MushroomState_Appear::~MushroomState_Appear()
{
}

void MushroomState_Appear::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(
		enMushroomAnimClip_Appear, 0.2f
	);
}

void MushroomState_Appear::Entry()
{
	//�A�N�V�������ɂ���
	m_mushroom->ActionActive();

	//�v���C���[�̕��������悤�ɂ���
	m_mushroom->TurnToPlayer();
}

void MushroomState_Appear::Ubdate()
{
	//�A�j���[�V�������I�������
	if (m_mushroom->GetModelRender().IsPlayingAnimation() == false)
	{
		m_mushroom->ProcessCommonTranstion();
	}
}

void MushroomState_Appear::Exit()
{
	//�A�N�V�������I���
	m_mushroom->ActionDeactive();
}
