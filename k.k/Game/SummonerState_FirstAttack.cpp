#include "stdafx.h"
#include "SummonerState_FirstAttack.h"
#include "Summoner.h"

void SummonerState_FirstAttack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_FirstAttack, 0.2f
	);
}

void SummonerState_FirstAttack::Entry()
{
	m_summoner->ActionActive();
	//ダメージ情報を設定する
	m_summoner->SettingDamageInfo(enSkillType_ComboFirst);

	m_isNextAttack = false;
}

void SummonerState_FirstAttack::Ubdate()
{
	m_summoner->UpdateComboAttackProcess();

	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートマシンを切り替えるのかチェック
		//次の攻撃に行くならフラグが立つ
		//なしにしてステートマシン側で決めてもいいかもしれない
		m_isNextAttack = m_summoner->CheckAndTransitionStateMachine();

		//共通ステート
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_FirstAttack::Exit()
{
	if (m_isNextAttack) return;

	m_summoner->ActionDeactive();
}
