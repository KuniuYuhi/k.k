#include "stdafx.h"
#include "HeroStateRun.h"
#include "Hero.h"

void HeroStateRun::ManageState()
{
	// ���ʂ̏�ԑJ�ڏ��������s����B
	m_hero->ProcessCommonStateTransition();
}

void HeroStateRun::PlayAnimation()
{
	//����A�j���[�V�����̍Đ�
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Run);
}
