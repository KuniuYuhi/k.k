#include "stdafx.h"
#include "HeroStateDash.h"
#include "Hero.h"

void HeroStateDash::ManageState()
{
	// ���ʂ̏�ԑJ�ڏ��������s����B
	m_hero->OnProcessDashStateTransition();
}

void HeroStateDash::PlayAnimation()
{
	//����A�j���[�V�����̍Đ�
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Dash,0.3f);
}
