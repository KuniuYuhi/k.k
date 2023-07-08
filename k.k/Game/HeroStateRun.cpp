#include "stdafx.h"
#include "HeroStateRun.h"
#include "Hero.h"

void HeroStateRun::ManageState()
{
	// 共通の状態遷移処理を実行する。
	m_hero->ProcessCommonStateTransition();
}

void HeroStateRun::PlayAnimation()
{
	//走るアニメーションの再生
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Run);
}
