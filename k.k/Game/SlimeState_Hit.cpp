#include "stdafx.h"
#include "SlimeState_Hit.h"
#include "Slime.h"

SlimeState_Hit::~SlimeState_Hit()
{
}

void SlimeState_Hit::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Hit, 0.2f, m_playAnimationResetFlag);

	m_playAnimationResetFlag = false;
}

void SlimeState_Hit::Entry()
{
	//�A�N�V�������ɂ���
	m_slime->ActionActive();

	m_slime->EntryHitActionProcess();

	m_playAnimationResetFlag = true;
}

void SlimeState_Hit::Ubdate()
{
	m_slime->UpdateHitActionProcess();

	////�A�j���[�V�������I�������
	//if (m_slime->GetModelRender().IsPlayingAnimation() == false)
	//{
	//	//�����d�����ċ��ʃX�e�[�g�����Ɉڍs

	//	m_slime->ProcessCommonTranstion();
	//}

}

void SlimeState_Hit::Exit()
{
	//�A�N�V�������I���
	m_slime->ActionDeactive();
}
