#include "stdafx.h"
#include "SummonerState_DarkSpearMain.h"
#include "Summoner.h"

#include "DarkSpear.h"

void SummonerState_DarkSpearMain::ManageState()
{
	if (m_createDarkSpearFag == true)
	{
		//ダークスピアを終わるアニメーションに移る
		m_summoner->SetNextAnimationState(
			Summoner::enAnimationState_DarkSpear_End);
		return;
	}

	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//地面から槍を生成仕切ったらエンドに遷移
		CreateDarkSpear();
		//
		m_createDarkSpearFag = true;
	}
}

void SummonerState_DarkSpearMain::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkSpear_Main,0.1f);
}

void SummonerState_DarkSpearMain::CreateDarkSpear()
{
	m_darkSpear = FindGO<DarkSpear>("darkspear");
	//槍を生成開始
	m_darkSpear->SetCreateStartFlag(true);
}
