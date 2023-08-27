#include "stdafx.h"
#include "HeroStateDash.h"
#include "Hero.h"

void HeroStateDash::ManageState()
{
	// 共通の状態遷移処理を実行する。
	m_hero->OnProcessDashStateTransition();
}

void HeroStateDash::PlayAnimation()
{
	//走るアニメーションの再生
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Dash,0.3f);
}
