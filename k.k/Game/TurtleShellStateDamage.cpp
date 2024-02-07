#include "stdafx.h"
#include "TurtleShellStateDamage.h"
#include "TurtleShell.h"

void TurtleShellStateDamage::ManageState()
{
	m_turtleShell->OnProcessDamageStateTransition();
}

void TurtleShellStateDamage::PlayAnimation()
{
	//�ŏ��̃A�j���[�V�����̍Đ��̓A�j���[�V���������Z�b�g
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Hit, 0.0f, m_animationResetFlag);
	//�A�j���[�V���������Z�b�g�����̂Ńt���O��������
	m_animationResetFlag = false;
}
