#include "stdafx.h"
#include "SlimeStateDamage.h"
#include "Slime.h"

void SlimeStateDamage::ManageState()
{
	m_slime->OnProcessDamageStateTransition();
}

void SlimeStateDamage::PlayAnimation()
{
	//�ŏ��̃A�j���[�V�����̍Đ��̓A�j���[�V���������Z�b�g
	m_slime->GetModelRender().PlayAnimation(enAnimationClip_Hit,0.0f, m_animationResetFlag);
	//�A�j���[�V���������Z�b�g�����̂Ńt���O��������
	m_animationResetFlag = false;
}
