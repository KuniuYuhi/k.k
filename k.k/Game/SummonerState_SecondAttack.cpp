#include "stdafx.h"
#include "SummonerState_SecondAttack.h"
#include "Summoner.h"

void SummonerState_SecondAttack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_SecondAttack, 0.2f
	);
}

void SummonerState_SecondAttack::Entry()
{
	m_summoner->ActionActive();
	//ダメージ情報を設定する
	m_summoner->SettingDamageInfo(enSkillType_ComboSecond);

}

void SummonerState_SecondAttack::Ubdate()
{
	m_summoner->UpdateComboAttackProcess();

	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートマシンを切り替えるのかチェック
		m_summoner->CheckAndTransitionStateMachine();
		//共通ステート
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_SecondAttack::Exit()
{
	m_summoner->ActionDeactive();
}
