#include "stdafx.h"
#include "SummonerSM_Attack.h"
#include "Summoner.h"
#include "ISummonerState.h"
#include "Player.h"
#include "CharactersInfoManager.h"

namespace {
	const float MELEE_ATTACK_RANGE = 280.0f;	//近距離攻撃の範囲内

	const float STAY_PLAYER_LIMMIT_TIME = 10.0f;		//プレイヤーが近くにとどまっているタイマーの上限

	const float DARK_METEO_ACTION_POINT = 30.0f;

}

EnActionName SummonerSM_Attack::m_oldActionName = enActionName_Idle;

void SummonerSM_Attack::Execute()
{
	//処理を止めているなら処理をしない
	if (m_summoner->IsStopProcessing() == true)
	{
		return;
	}

	ProcessDecideAction();
	
}

void SummonerSM_Attack::Init(bool resetoldActionFlag)
{
	if (resetoldActionFlag == true)
	{
		//前のアクションをリセット
		m_oldActionName = enActionName_Idle;
	}
}

void SummonerSM_Attack::ProcessDecideAction()
{
	//一定間隔で攻撃。ある条件で待機状態でも攻撃に移行
	//サモナーが攻撃中は処理しない
	if (m_summoner->IsAttackEnable() == false)
	{
		return;
	}
	//もう一度行動するときは
	if (m_continuousAttackTimer <= 0.0f)
	{
		//これから行動を決める
		DecideNextAction();
	}
	else
	{
		//もう一度攻撃タイマーを加算
		m_continuousAttackTimer -= g_gameTime->GetFrameDeltaTime();
	}
	
}

void SummonerSM_Attack::DecideNextAction()
{
	//どんな攻撃にするか決める

	//サモナーから勇者に向かうベクトルを計算する
	m_toPlayer = m_summoner->GetPosition() -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();

	//距離によって近距離攻撃か遠距離攻撃か決める
	if (m_toPlayer.Length() < MELEE_ATTACK_RANGE)
	{
		//近距離攻撃
		ProcessMeleeAttack();
	}
	else
	{
		//遠距離攻撃
		ProcessLongRangeAttack();
	}

	//連続攻撃回数を増やす
	m_continuousAttackCount++;
	//起こしたアクションを記憶
	m_oldActionName = m_summoner->GetNowSummonerState()->GetActionName();
}

void SummonerSM_Attack::ProcessLongRangeAttack()
{
	//遠距離攻撃

	//ダークメテオ
	/*m_summoner->
		SetNextAnimationState(
			Summoner::enAnimationState_Attack_DarkMeteorite_start);
	return;*/

	//前の行動がダークメテオでないなら
	//ダークメテオを撃つか調べる
	if (IsNextActionDarkMeteo() == true && 
		m_oldActionName != enActionName_DarkMeteo_Start)
	{
		//ダークメテオ
		m_summoner->
			SetNextAnimationState(
				Summoner::enAnimationState_Attack_DarkMeteorite_start
			);
	}
	else
	{
		//ダークボール
		IsNextActionDarkBall();
	}

	//遠距離攻撃行動した後の連続攻撃タイマー
	m_continuousAttackTimer = 1.0f;
	
}

void SummonerSM_Attack::ProcessMeleeAttack()
{
	//通常攻撃
	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
	//通常攻撃行動した後の連続攻撃タイマー
	m_continuousAttackTimer = 0.2f;
	return;
	//近距離攻撃
	//m_summoner->
	//	SetNextAnimationState(Summoner::enAnimationState_DarkSpear_Start);
	////ダークスピア攻撃行動した後の連続攻撃タイマー
	//m_continuousAttackTimer = 1.5f; 
	//return;

	//近距離攻撃カウントを増やす
	m_meleeCount++;

	int bb = rand() % 10;
	//確率で変化
	if (bb > 7)
	{
		//ダークスピア
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkSpear_Start);
		//ダークスピア攻撃行動した後の連続攻撃タイマー
		m_continuousAttackTimer = 1.5f;
	}
	else if (bb > 4)
	{
		//ダークウォール
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkWall);
		//ダークウォール攻撃行動した後の連続攻撃タイマー
		m_continuousAttackTimer = 1.0f;
	}
	else
	{
		//通常攻撃
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
		//通常攻撃行動した後の連続攻撃タイマー
		m_continuousAttackTimer = 0.2f;
	}

}

void SummonerSM_Attack::IsNextActionDarkBall()
{
	//二連続で撃ったり、追尾するダークボールにしたりする
	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_DarkBall);

	if (m_lotteryChanceFlag != true)
	{
		//もう一度攻撃するチャンスあり
		m_lotteryChanceFlag = true;
	}
	else
	{
		//もう一度攻撃しているのでフラグをリセット
		m_lotteryChanceFlag = false;
	}
}

bool SummonerSM_Attack::IsNextActionDarkMeteo()
{
	//このポイントが一定以上になったら、次の行動はダークメテオにする
	float point = 0;

	//プレイヤーの座標
	Vector3 playerPos = CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//離れている距離をポイントに換算する
	Vector3 diff = m_summoner->GetPosition() - playerPos;
	//ポイントを加算。ベクトルの十分の一の長さを加算
	point += (diff.Length() / 100);

	//プレイヤーの周りにモブモンスターがどのくらいいないか確かめる
	int count = 0;
	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		Vector3 mobToPlayer = playerPos - monster->GetPosition();
		//プレイヤーから一定の距離内にモンスターがいないなら
		if (mobToPlayer.Length() > 300.0f)
		{
			//カウントを加算
			count++;
		}
	}

	//範囲内にいないカウントをポイントに加算
	point += count;

	//被ダメージカウントをポイントに加算
	point += m_summoner->GetDamageCount();

	//計算したポイントが一定値をこえたら
	if (point >= DARK_METEO_ACTION_POINT)
	{
		//ダークメテオを撃つのでカウントをリセット
		m_summoner->SetDamageCount(0);
		return true;
	}
	//ダークメテオを撃たない
	return false;
}
