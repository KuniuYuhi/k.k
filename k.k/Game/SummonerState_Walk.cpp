#include "stdafx.h"
#include "SummonerState_Walk.h"
#include "Summoner.h"

void SummonerState_Walk::ManageState()
{
	//�ړ�����
	//m_summoner->ProcessMove();

	m_summoner->ProcessCommonStateTransition();
}

void SummonerState_Walk::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Walk);
}
