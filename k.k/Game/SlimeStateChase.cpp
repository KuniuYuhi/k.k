#include "stdafx.h"
#include "SlimeStateChase.h"
#include "Slime.h"

void SlimeStateChase::ManageState()
{
	//�v���C���[��ǂ�������
	m_slime->MoveChasePlayer(m_slime->GetCharacterController());

	//m_slime->ProcessCommonStateTransition();
}

void SlimeStateChase::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAninationClip_Chase,0.1f);
}