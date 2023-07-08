#include "stdafx.h"
#include "HeroStateIdle.h"
#include "Hero.h"

void HeroStateIdle::ManageState()
{
	// 共通の状態遷移処理を実行する。
	m_hero->ProcessCommonStateTransition();
}
void HeroStateIdle::PlayAnimation()
{
	//待機アニメーションの再生
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Idle, 0.3f);
}
