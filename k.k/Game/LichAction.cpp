#include "stdafx.h"
#include "LichAction.h"
#include "Lich.h"
#include "Player.h"

int LichAction::NextAction()
{
	//評価値初期化
	for (int i=0;i< m_actionAmount;i++)
	{
		m_action[i].m_eval = 0;
	}

	//m_eval[m_actionAmount];

	for (int i = 0; i < m_actionAmount; i++)
	{
		m_eval[i] = 0;
	}


	CalcEvalAttack1(enAttack_1/*m_action[enAttack_1]*/);

	CalcEvalAttack2(enAttack_2/*m_action[enAttack_2]*/);

	CalcEvalSummon(enAttack_Summon);

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
		//どんな行動をしたか保存する
		m_oldActionNumber = enAttack_1;
		return 0;
		break;
	case enAttack_2:
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_2);
		//どんな行動をしたか保存する
		m_oldActionNumber = enAttack_2;
		return 0;
		break;
	case enAttack_DarkMeteorite:
		//この技は無敵状態で打つ
		m_lich->SetInvincibleFlag(true);
		//ワープ
		m_lich->Warp(Lich::enSpecialActionState_Warp);
		//特別な行動ステートを元に戻す
		m_lich->SetSpecialActionState(Lich::enSpecialActionState_Normal);
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_DarkMeteorite_start);
		//どんな行動をしたか保存する
		m_oldActionNumber = enAttack_DarkMeteorite;
		return 0;
		break;
	case enAttack_Summon:
		//ワープ
		m_lich->Warp(Lich::enSpecialActionState_CenterWarp);
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAninationState_Summon);
		//蓄積した値を0にする
		m_lich->SetHitCountAndDamage(0, 0);
		
		//どんな行動をしたか保存する
		m_oldActionNumber = enAttack_Summon;
		return 0;


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
		if (MaxEval < m_action[i].m_eval)
		{
			MaxEval = m_action[i].m_eval;
			NextActionNomber = i;
		}

		/*if (MaxEval < m_eval[i])
		{
			MaxEval = m_eval[i];
			NextActionNomber = i;
		}*/
	}
	return NextActionNomber;
}

void LichAction::CalcEvalAttack1(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//攻撃範囲内にプレイヤーがいたら
	if(m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_1Distance)==true)
	{
		m_action[m_enActionNumber].m_eval += 100;
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
		m_action[m_enActionNumber].m_eval += 120;
		m_eval[m_enActionNumber] += 120;
	}
	else
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}

void LichAction::CalcEvalSummon(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//既に6体以上モンスターがいたらこの行動をさせない
	if (m_lich->GetAIActors() >= 6)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}

	//自身の状況がよくないときに召喚
	//座標
	//5回攻撃されたら
	//カウント分10ずつ増やす
	//被ダメージした回数評価値をたす
	for (int i = 0; i < m_lich->GetHitCount(); i++)
	{
		m_action[m_enActionNumber].m_eval += 10;
		m_eval[m_enActionNumber] += 10;
	}

	//最大HPの1/5のダメージが蓄積したら評価値を増やす
	if (m_lich->GetAccumulationDamage()>m_lich->GetStatus().maxHp/5)
	{
		m_action[m_enActionNumber].m_eval += 50;
		m_eval[m_enActionNumber] += 40;
	}
	//他の条件

}

void LichAction::CalcEvalDarkMeteorite(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//ワープしたなら
	if (m_lich->GetSpecialActionState() == Lich::enSpecialActionState_Warp)
	{
		m_action[m_enActionNumber].m_eval = INT_MAX;
		m_eval[m_enActionNumber] += INT_MAX;
	}
	else
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}



void LichAction::CalcEvalIdle(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//何回かに一回何もしない

}
