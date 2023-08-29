#include "stdafx.h"
#include "LichAction.h"
#include "Lich.h"
#include "Player.h"

//行動の優先度つける
void LichAction::SettingPriority()
{
	m_action[enAttack_None].m_priority = 0;
	m_action[enAttack_1].m_priority = 1;
	m_action[enAttack_2].m_priority = 2;
	m_action[enAttack_DarkMeteorite].m_priority = 4;
	m_action[enAttack_Summon].m_priority = 3;
	m_action[enAttack_AngryMode].m_priority = 0;
}

int LichAction::NextAction()
{
	//評価値初期化
	/*for (int i=0;i< m_actionAmount;i++)
	{
		m_action[i].m_eval = 0;
	}*/
	CalcEvalIdle(enAttack_None);

	CalcEvalAttack1(enAttack_1);

	CalcEvalAttack2(enAttack_2);

	CalcEvalSummon(enAttack_Summon);

	CalcEvalDarkMeteorite(enAttack_DarkMeteorite);

	CalcEvalAngryMode(enAttack_AngryMode);

	//評価値が一番の配列の番号を取得
	int NextActionNomber = CalcMaxEvalNumber();


	switch (NextActionNomber)
	{
	case enAttack_None:
		//何もしない(移動)
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
		//特別な行動ステートを元に戻す
		//m_lich->SetSpecialActionState(Lich::enSpecialActionState_Normal);
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_DarkMeteorite_start);
		//どんな行動をしたか保存する
		m_oldActionNumber = enAttack_DarkMeteorite;
		//評価値リセット
		m_action[NextActionNomber].m_eval = 0;

		return 0;
		break;
	case enAttack_Summon:
		//この技は無敵状態で打つ
		m_lich->SetInvincibleFlag(true);
		//ワープ
		//m_lich->Warp(Lich::enSpecialActionState_CenterWarp);
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAninationState_Summon);
		//蓄積した値を0にする
		m_lich->SetHitCountAndDamage(0, 0);
		
		//どんな行動をしたか保存する
		m_oldActionNumber = enAttack_Summon;
		return 0;

	case enAttack_AngryMode:
		//この技は無敵状態で打つ
		m_lich->SetInvincibleFlag(true);
		//ステートセット
		m_lich->SetNextAnimationState(Lich::enAnimationState_Angry);

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
	//todo ネスト気になる
	for (int i = 0; i < m_actionAmount; i++)
	{
		if (MaxEval < m_action[i].m_eval)
		{
			//評価値が同じもの同士なら
			//優先度で決める
			/*if (MaxEval == m_action[i].m_eval)
			{
				if (m_action[NextActionNomber].m_priority > m_action[i].m_priority)
				{
					continue;
				}
			}*/

			//最大値更新
			MaxEval = m_action[i].m_eval;
			NextActionNomber = i;
		}
	}
	return NextActionNomber;
}

void LichAction::CalcEvalAttack1(EnActionNumber m_enActionNumber)
{
	//攻撃範囲内にプレイヤーがいたら
	if(m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_1Distance)==true)
	{
		m_action[m_enActionNumber].m_eval = 100;
	}
	else
	{
		m_eval[m_enActionNumber] += INT_MIN;
	}
}

void LichAction::CalcEvalAttack2(EnActionNumber m_enActionNumber)
{
	//攻撃範囲内にプレイヤーがいたら
	if (m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_2Distance) == true)
	{
		m_action[m_enActionNumber].m_eval = 120;
	}
	else
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
	}
}

void LichAction::CalcEvalSummon(EnActionNumber m_enActionNumber)
{
	//初期化
	m_action[m_enActionNumber].m_eval = 0;

	//最初のアクションなら
	if (m_firstActionNumber == m_enActionNumber)
	{
		m_action[m_enActionNumber].m_eval = INT_MAX;
		m_firstActionNumber = enAttack_None;
		return;
	}


	//既に6体以上モンスターがいたらこの行動をさせない
	if (m_lich->GetAIActors() >= 6)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}
	//既に怒りモードならこの行動にさせない
	if (m_lich->GetSpecialActionState() == Lich::enSpecialActionState_AngryMode)
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
	}

	//最大HPの1/5のダメージが蓄積したら評価値を増やす
	if (m_lich->GetAccumulationDamage()>m_lich->GetStatus().maxHp/5)
	{
		m_action[m_enActionNumber].m_eval += 50;
	}
	//他の条件

}

void LichAction::CalcEvalDarkMeteorite(EnActionNumber m_enActionNumber)
{
	//二回連続同じ行動にしない
	if (m_oldActionNumber == enAttack_DarkMeteorite)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}
	//時間が30秒たったら

	//todo 条件
	m_action[m_enActionNumber].m_eval += 5;
	
	if (m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_2Distance) == false)
	{
		m_action[m_enActionNumber].m_eval += 5;
	}

	
}

void LichAction::CalcEvalAngryMode(EnActionNumber m_enActionNumber)
{
	//怒りモード中なら処理しない
	//既に怒りモードならこの行動にさせない
	if (m_lich->GetSpecialActionState() == Lich::enSpecialActionState_AngryMode)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}

	//ダメージが溜まったら怒りモード
	if (m_lich->GetAngryModeCount() >= 3)
	{
		m_action[m_enActionNumber].m_eval = INT_MAX;
	}

}



void LichAction::CalcEvalIdle(EnActionNumber m_enActionNumber)
{
	//何もしない(移動)

}
