#include "stdafx.h"
#include "SummonerSM_Vigilance.h"
#include "Summoner.h"
#include "ISummonerState.h"
#include "Player.h"
#include "CharactersInfoManager.h"

namespace {
	const float WAIT_TIME = 500.0f;		//待機時間

	const float MELEE_ATTACK_RANGE = 280.0f;	//近距離攻撃の範囲内

	const float STAY_PLAYER_LIMMIT_TIME = 500.0f;		//プレイヤーが近くにとどまっているタイマーの上限


	const float KNOCKBACK_DISTANCE = 200.0f;

	const float DARK_METEO_ACTION_POINT = 40.0f;

}

SummonerSM_Vigilance::~SummonerSM_Vigilance()
{
}

void SummonerSM_Vigilance::Execute()
{
	ProcessDecideAction();
}

void SummonerSM_Vigilance::Init(bool saveTimerlesetFlag)
{
	if (saveTimerlesetFlag == true)
	{
		//タイマーをリセット
		m_stayPlayerTimer = 0.0f;
	}	
}

void SummonerSM_Vigilance::ProcessDecideAction()
{
	//まず待機ステートか攻撃ステートか決める
	//待機時間の間またはもう一度攻撃するチャンスがないなら
	if (m_waitTimer < WAIT_TIME)
	{
		//待機ステートを決める
		ProcessWaitState();
	}
	else
	{
		//攻撃ステートマシンに切り替える
		m_summoner->SetNextStateMachine(Summoner::enStateMachineState_Attack);
		return;
	}
}

void SummonerSM_Vigilance::ProcessWaitState()
{
	//タイマーを加算
	m_waitTimer += g_gameTime->GetFrameDeltaTime();

	//ノックバックするか決める、決まったら即ノックバック
	ProcessIsKnockBack();
}

void SummonerSM_Vigilance::ProcessIsKnockBack()
{
	//サモナーから勇者に向かうベクトルを計算する
	m_toPlayer = m_summoner->GetPosition() -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();


	//長い間勇者が近くにいるなら
	//逃げるかノックバックで引き離す
	if (m_toPlayer.Length() < KNOCKBACK_DISTANCE)
	{
		//タイマーを加算。攻撃させたりするとタイマーがさらに加速
		AddStayPlayerTimer();
	}
	else
	{
		//範囲外ならタイマーリセット
		m_stayPlayerTimer = 0.0f;
	}

	//とどまっている時間が上限をこえたら
	if (m_stayPlayerTimer > STAY_PLAYER_LIMMIT_TIME)
	{
		//とどまっているタイマーリセット
		m_stayPlayerTimer = 0.0f;
		//ノックバックした後すぐ攻撃しないように一秒は空ける
		if (m_waitTimer > 1.0f)
		{
			m_waitTimer = 1.0f;
		}

		//ノックバックする
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		return;

	}
}

void SummonerSM_Vigilance::IsKnockBackTimerAccelerate()
{
}

void SummonerSM_Vigilance::AddStayPlayerTimer()
{
	//条件によってタイマーを加速させる
	IsKnockBackTimerAccelerate();
	//タイマーを加算
	m_stayPlayerTimer += g_gameTime->GetFrameDeltaTime() * m_accelerateStayPlayerTimer;



	//何らかの要因でさらにタイマーを加速-＞m_accelerateStayPlayerTimer
}
