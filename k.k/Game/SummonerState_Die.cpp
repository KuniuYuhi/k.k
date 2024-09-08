#include "stdafx.h"
#include "SummonerState_Die.h"
#include "Summoner.h"

#include "GameSceneManager.h"

void SummonerState_Die::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(enSummonerAnimClip_Die, 0.2f);
}

void SummonerState_Die::Entry()
{
	//アニメーション速度を遅くする
	m_summoner->GetModelRender().SetAnimationSpeed(0.5f);

	m_summoner->ActionActive();
	//ここに入った時点で勝敗は決まっているので
	//プレイヤーが勝ったことを伝える
	GameSceneManager::GetInstance()->SetOutComePlayerWin();

}

void SummonerState_Die::Ubdate()
{
	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//死亡後の処理
		m_summoner->AfterDieProcess();
	}
}

void SummonerState_Die::Exit()
{

	m_summoner->ActionDeactive();
}
