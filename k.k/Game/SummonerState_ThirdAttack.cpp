#include "stdafx.h"
#include "SummonerState_ThirdAttack.h"
#include "Summoner.h"

void SummonerState_ThirdAttack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_ThirdAttack, 0.2f
	);
}

void SummonerState_ThirdAttack::Entry()
{
	m_summoner->ActionActive();
	//ダメージ情報を設定する
	m_summoner->SettingDamageInfo(enSkillType_ComboThird);
}

void SummonerState_ThirdAttack::Ubdate()
{
	
	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートマシンを切り替えるのかチェック
		m_summoner->CheckAndTransitionStateMachine();
		//共通ステート
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_ThirdAttack::Exit()
{
	m_summoner->ActionDeactive();
}
