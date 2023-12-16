#include "stdafx.h"
#include "SummonerState_DarkBall.h"
#include "Summoner.h"

#include "DarkBall.h"

//todo �T���i�[�̒��g�����������Ȃ�


SummonerState_DarkBall::~SummonerState_DarkBall()
{
	/*m_summoner->GetModelRender().RemoveAnimationEvent(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		}
	);*/
}

void SummonerState_DarkBall::Init()
{
	static bool m_initFlag = false;

	if (m_initFlag == true)
	{
		return;
	}

	m_summoner->GetModelRender().AddAnimationEvent(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_initFlag = true;
}

void SummonerState_DarkBall::ManageState()
{
	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_DarkBall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkBall);
}

void SummonerState_DarkBall::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�_�[�N�{�[�������^�C�~���O
	if (wcscmp(eventName, L"Create_Ball") == 0)
	{
		m_summoner = FindGO<Summoner>("summoner");
		//�{�[������
		CreateDarkBall();
	}
}

void SummonerState_DarkBall::CreateDarkBall()
{
	DarkBall* darkBall = NewGO<DarkBall>(0, "darkball");
	darkBall->SetAtk(m_summoner->GetStatus().GetAtk());
	darkBall->Setting(m_summoner->GetPosition(), m_summoner->GetRotation());


}
