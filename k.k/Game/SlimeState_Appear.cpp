#include "stdafx.h"
#include "SlimeState_Appear.h"
#include "Slime.h"

SlimeState_Appear::~SlimeState_Appear()
{

}

void SlimeState_Appear::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Appear, 0.2f);
}

void SlimeState_Appear::Entry()
{
	//�A�N�V�������ɂ���
	m_slime->ActionActive();

	//�v���C���[�̕��������悤�ɂ���
	m_slime->TrunToTarget();

}

void SlimeState_Appear::Ubdate()
{
	//�A�j���[�V�������I�������
	if (m_slime->GetModelRender().IsPlayingAnimation() == false)
	{
		m_slime->ProcessCommonTranstion();
	}

}

void SlimeState_Appear::Exit()
{
	//�A�N�V�������I���
	m_slime->ActionDeactive();
}
