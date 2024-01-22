#include "stdafx.h"
#include "SummonerState_KnockBack.h"
#include "Summoner.h"

#include "CharactersInfoManager.h"

namespace {
	const float KNOCKBACK_RANGE = 300.0f;
	const float KNOCKBACK_POWER = 400.0f;
	const float KNOCKBACK_LIMMIT = 2.0f;
}

SummonerState_KnockBack::~SummonerState_KnockBack()
{
}

void SummonerState_KnockBack::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�v���C���[���m�b�N�o�b�N�������̃t���O�����Z�b�g
		m_summoner->SetPlayerKnockedBackFlag(false);
		//���ʂ̏�ԑJ�ڏ���
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_KnockBack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_KnockBack);
}

void SummonerState_KnockBack::ProcessKnockBack()
{
	//�v���C���[���͈͓��ɂ�����m�b�N�o�b�N����悤�ɂ���
	Vector3 diff = m_summoner->GetPosition() -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//�m�b�N�o�b�N�͈͓��Ȃ�
	if (diff.Length() < KNOCKBACK_RANGE)
	{
		CharactersInfoManager::GetInstance()->
			GetPlayerInstance()->SetKnockBackInfo(
				true, m_summoner->GetPosition(), KNOCKBACK_POWER, KNOCKBACK_LIMMIT);
	}
}
