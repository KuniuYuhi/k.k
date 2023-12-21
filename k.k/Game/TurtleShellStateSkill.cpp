#include "stdafx.h"
#include "TurtleShellStateSkill.h"
#include "TurtleShell.h"

void TurtleShellStateSkill::ManageState()
{
	//�h�䂷�鎞�Ԃ��I�������A�j���[�V�����I���	
	if (m_turtleShell->IsDifenceEnd() == true)
	{
		//���ʂ̃X�e�[�g�J�ڏ������s
		m_turtleShell->ProcessCommonStateTransition();
	}
}

void TurtleShellStateSkill::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Skill);
}
