#include "stdafx.h"
#include "LichAction.h"
#include "Lich.h"
#include "Player.h"

int LichAction::NextAction()
{
	//評価値初期化
	/*for (int i=0;i< m_actionAmount;i++)
	{
		m_action[i].m_eval = 0;
	}*/

	//m_eval[m_actionAmount];

	for (int i = 0; i < m_actionAmount; i++)
	{
		m_eval[i] = 0;
	}


	CalcEvalAttack1(enAttack_1/*m_action[enAttack_1]*/);

	CalcEvalAttack2(enAttack_2/*m_action[enAttack_2]*/);

	CalcEvalDarkMeteorite(enAttack_DarkMeteorite/*m_action[enAttack_3]*/);


	//評価値が一番の配列の番号を取得
	int NextActionNomber = CalcMaxEvalNumber();


	switch (NextActionNomber)
	{
	case enAttack_None:
		//何もしない
		return 0;
		break;
	case enAttack_1:
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_1);
		return 0;
		break;
	case enAttack_2:
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_2);
		return 0;
		break;
	case enAttack_DarkMeteorite:
		//ステートセット
		m_lich->SetInvincibleFlag(true);
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_DarkMeteorite_start);
		return 0;
		break;
	default:
		return 0;
		break;
	}



	return 0;
}

int LichAction::CalcMaxEvalNumber()
{
	int MaxEval = 0;
	int NextActionNomber = 0;

	//一番高い評価値を探す
	for (int i = 0; i < m_actionAmount; i++)
	{
		if (MaxEval < m_eval[i])
		{
			MaxEval = m_eval[i];
			NextActionNomber = i;
		}
	}
	return NextActionNomber;
}

void LichAction::CalcEvalAttack1(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//攻撃範囲内にプレイヤーがいたら
	if(m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_1Distance)==true)
	{
		//action.m_eval += 100;
		m_eval[m_enActionNumber] += 100;
	}
	else
	{
		//action.m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}

void LichAction::CalcEvalAttack2(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//攻撃範囲内にプレイヤーがいたら
	if (m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_2Distance) == true)
	{
		//action.m_eval += 120;
		m_eval[m_enActionNumber] += 120;
	}
	else
	{
		//action.m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}

void LichAction::CalcEvalSummon(EnActionNumber m_enActionNumber/*Action& action*/)
{
}

void LichAction::CalcEvalDarkMeteorite(EnActionNumber m_enActionNumber/*Action& action*/)
{
	if (m_lich->GetSpecialActionState() == Lich::enSpecialActionState_Warp)
	{
		//action.m_eval = INT_MAX;
		m_eval[m_enActionNumber] += INT_MAX;
	}
	else
	{
		//action.m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}



void LichAction::CalcEvalIdle(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//何回かに一回何もしない

}
