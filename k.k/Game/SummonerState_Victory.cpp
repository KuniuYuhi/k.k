#include "stdafx.h"
#include "SummonerState_Victory.h"
#include "Summoner.h"
#include "GameManager.h"

void SummonerState_Victory::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//モンスターの勝利！
		GameManager::GetInstance()->SetGameFinishProcessEndFlag(true);
	}
}

void SummonerState_Victory::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Victory);
}
