#include "stdafx.h"
#include "HeroStateWalk.h"
#include "Hero.h"

void HeroStateWalk::ManageState()
{
	// ���ʂ̏�ԑJ�ڏ��������s����B
	m_hero->ProcessCommonStateTransition();
}

void HeroStateWalk::PlayAnimation()
{
	//�����A�j���[�V�����̍Đ�
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Walk,0.3f);
}
