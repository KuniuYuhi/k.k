#include "stdafx.h"
#include "SummonerState_DarkSpearStart.h"
#include "Summoner.h"
#include "DarkSpear.h"

void SummonerState_DarkSpearStart::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//ダークスピアを生成する場所を決める
		DarkSpear* darkSpear = NewGO<DarkSpear>(0, "darkspear");
		darkSpear->SetPosition(m_summoner->GetPosition());
		darkSpear->SetForward(m_summoner->GetForward());
		//ダークスピアメインアニメーションに移る
		m_summoner->SetNextAnimationState(
			Summoner::enAnimationState_DarkSpear_Main);
	}
}

void SummonerState_DarkSpearStart::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimationState_DarkSpear_Start, 0.1f);
}
