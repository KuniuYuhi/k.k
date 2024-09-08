#include "stdafx.h"
#include "SummonerState_Idle.h"
#include "Summoner.h"

void SummonerState_Idle::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Idle, 0.2f
	);
}

void SummonerState_Idle::Entry()
{
	//�ڋ߂ł��鋗���ɖ����B
	m_summoner->DeactiveArrivedApproachDistance();


}

void SummonerState_Idle::Ubdate()
{
	//�ړ��ʂ��������������ԂɑJ��
	if (fabsf(m_summoner->GetMoveSpeed().x) >= 0.001f ||
		fabsf(m_summoner->GetMoveSpeed().z) >= 0.001f)
	{
		m_summonerStateContext->ChangeState(enSummonerState_Run);
		return;
	}
}

void SummonerState_Idle::Exit()
{
}
