#include "stdafx.h"
#include "HeroStateWalk.h"
#include "Hero.h"

void HeroStateWalk::ManageState()
{
	// 共通の状態遷移処理を実行する。
	m_hero->ProcessCommonStateTransition();
}

void HeroStateWalk::PlayAnimation()
{
	//歩きアニメーションの再生
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Walk,0.3f);
}
