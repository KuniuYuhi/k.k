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
				//攻撃まで待機
				m_summoner->ProcessCommonStateTransition();
				return;
			}
		}
		//攻撃ステートが終わった後の処理。todo ここで全部処理したい
		m_summoner->ProcessEndAttackState();

	}
}

void SummonerState_DarkBall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkBall);
}
