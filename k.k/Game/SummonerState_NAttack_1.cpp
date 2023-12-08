#include "stdafx.h"
#include "SummonerState_NAttack_1.h"
#include "Summoner.h"

#include "Player.h"
#include "CharactersInfoManager.h"


namespace {
	const float RANGE = 230.0f;

	const int MAX_PROBABILITY = 1;		//確率の最大値

	const int ATTACK_MAX_PROBABILITY = 5;
}


void SummonerState_NAttack_1::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		NextAction();
	}
}

void SummonerState_NAttack_1::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_NormalAttack_1);
}

bool SummonerState_NAttack_1::IsPlayerInRange()
{
	//ボスからプレイヤーに向かうベクトルを計算
	Vector3 diff =
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition() -
		m_summoner->GetPosition();

	//ベクトルの長さが範囲内なら
	if (diff.Length() < RANGE)
	{
		//いる
		return true;
	}
	//いない
	return false;
}

void SummonerState_NAttack_1::NextAction()
{
	//乱数を初期化。
	srand((unsigned)time(NULL));

	//攻撃後にまだプレイヤーが範囲内にいるなら
	if (IsPlayerInRange() == true)
	{
		//攻撃する確率を確定にする
		m_probability = MAX_PROBABILITY;
	}

	//コンボするか待機にするか決める
	int answer = rand() % m_probability;

	//結果によって行動変化
	if (answer < ATTACK_MAX_PROBABILITY)
	{
		//次の攻撃に遷移
		m_summoner->SetNextAnimationState(Summoner::enAnimationState_NormalAttack_2);
	}
	else
	{
		//待機状態に遷移
		m_summoner->ProcessCommonStateTransition();
	}
}
