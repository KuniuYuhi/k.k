#include "stdafx.h"
#include "HeroStateIdle.h"
#include "Hero.h"

void HeroStateIdle::ManageState()
{
	// ���ʂ̏�ԑJ�ڏ��������s����B
	m_hero->ProcessCommonStateTransition();
}
void HeroStateIdle::PlayAnimation()
{
	//�ҋ@�A�j���[�V�����̍Đ�
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Idle, 0.3f);
}
