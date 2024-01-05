#include "stdafx.h"
#include "SummonerState_DarkBall.h"
#include "Summoner.h"

#include "DarkBall.h"

//todo サモナーの中身がおかしくなる
namespace {
	const int CONTINUOUS_ATTACK_COUNT_LIMMIT = 2;
	const int ONE_MORE_ATTACK_PROBABILITY = 5;
}

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
	/*static bool m_initFlag = false;

	if (m_initFlag == true)
	{
		return;
	}

	m_summoner->GetModelRender().AddAnimationEvent(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_initFlag = true;*/
}

void SummonerState_DarkBall::ManageState()
{
	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		

		//連続攻撃回数が上限以下なら
		if (m_summoner->GetNowStateMachine()->GetContinuousAttackCount() <
			CONTINUOUS_ATTACK_COUNT_LIMMIT)
		{
			//もう一度攻撃する確率を決める
			int num = rand() % 10;
			//もう一度攻撃する確率を上回ったら攻撃する
			if (num > ONE_MORE_ATTACK_PROBABILITY)
			{
				m_summoner->ProcessCommonStateTransition();
				return;
			}
		}
		//攻撃ステートが終わった後の処理
		m_summoner->ProcessEndAttackState();

	}
}

void SummonerState_DarkBall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkBall);
}

//void SummonerState_DarkBall::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
//{
//	//ダークボール生成タイミング
//	if (wcscmp(eventName, L"Create_Ball") == 0)
//	{
//		m_summoner = FindGO<Summoner>("summoner");
//		//ボール生成
//		CreateDarkBall();
//	}
//}

void SummonerState_DarkBall::CreateDarkBall()
{
	DarkBall* darkBall = NewGO<DarkBall>(0, "darkball");
	darkBall->SetAtk(m_summoner->GetStatus().GetAtk());
	darkBall->Setting(m_summoner->GetPosition(), m_summoner->GetRotation());


}
