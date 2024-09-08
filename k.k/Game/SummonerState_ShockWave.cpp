#include "stdafx.h"
#include "SummonerState_ShockWave.h"
#include "Summoner.h"

void SummonerState_ShockWave::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_ShockWave, 0.2f
	);
}

void SummonerState_ShockWave::Entry()
{
	m_summoner->ActionActive();
	//ダメージ情報を設定する
	m_summoner->SettingDamageInfo(enSkillType_ShockWave);
}

void SummonerState_ShockWave::Ubdate()
{
	m_summoner->UpdateShockWaveProcess();


	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートマシンを切り替えるのかチェック
		m_summoner->CheckAndTransitionStateMachine();
		//共通ステート
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_ShockWave::Exit()
{
	m_summoner->ActionDeactive();
}
