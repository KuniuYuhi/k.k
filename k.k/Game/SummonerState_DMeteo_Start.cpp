#include "stdafx.h"
#include "SummonerState_DMeteo_Start.h"
#include "Summoner.h"

#include "DarkMeteo.h"

void SummonerState_DMeteo_Start::ManageState()
{
	//アニメーションが終わったらチャージ。
	//チャージが終わったら次のアニメーション、ステート

	if (m_darkMeteo != nullptr)
	{
		//チャージが終わったら
		if (m_darkMeteo->GetChargeCompleteFlag() == true)
		{
			//メテオを撃つアニメーションに進む
			m_summoner->SetNextAnimationState(
				Summoner::enAnimationState_Attack_DarkMeteorite_main);
		}
		return;
	}



	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//アニメーションが終わったので、ダークメテオのチャージ開始
		m_darkMeteo = NewGO<DarkMeteo>(0, "darkmeteo");
		m_darkMeteo->SetDarkMeteoPosition(m_summoner->GetPosition());

	}
}

void SummonerState_DMeteo_Start::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Attack_DarkMeteorite_start);
}
