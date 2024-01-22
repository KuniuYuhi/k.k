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
		//プレイヤーをノックバックしたかのフラグをリセット
		m_summoner->SetPlayerKnockedBackFlag(false);
		//共通の状態遷移処理
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_KnockBack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_KnockBack);
}

void SummonerState_KnockBack::ProcessKnockBack()
{
	//プレイヤーが範囲内にいたらノックバックするようにする
	Vector3 diff = m_summoner->GetPosition() -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//ノックバック範囲内なら
	if (diff.Length() < KNOCKBACK_RANGE)
	{
		CharactersInfoManager::GetInstance()->
			GetPlayerInstance()->SetKnockBackInfo(
				true, m_summoner->GetPosition(), KNOCKBACK_POWER, KNOCKBACK_LIMMIT);
	}
}
