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

void SummonerState_KnockBack::Init()
{
	static bool m_KnockBackinitFlag = false;

	if (m_KnockBackinitFlag == true)
	{
		return;
	}

	m_summoner->GetModelRender().AddAnimationEvent(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});
	m_KnockBackinitFlag = true;
}

void SummonerState_KnockBack::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_KnockBack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_KnockBack);
}

void SummonerState_KnockBack::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//タイミング
	if (wcscmp(eventName, L"KnockBack") == 0)
	{
		m_summoner = FindGO<Summoner>("summoner");
		//ボール生成
		ProcessKnockBack();
	}
	
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
