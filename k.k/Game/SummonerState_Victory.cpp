#include "stdafx.h"
#include "SummonerState_Victory.h"
#include "Summoner.h"
#include "GameManager.h"

void SummonerState_Victory::ManageState()
{
	//�����X�^�[�̏����I
	GameManager::GetInstance()->SetGameFinishProcessEndFlag(true);

	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		
	}
}

void SummonerState_Victory::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Victory);
}
