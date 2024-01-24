#include "stdafx.h"
#include "SummonerSM_Vigilance.h"
#include "Summoner.h"
#include "ISummonerState.h"
#include "Player.h"
#include "CharactersInfoManager.h"

namespace {
	const float WAIT_TIME = 4.0f;		//待機時間

	const float MELEE_ATTACK_RANGE = 280.0f;	//近距離攻撃の範囲内

	const float STAY_PLAYER_LIMMIT_TIME = 5.0f;		//プレイヤーが近くにとどまっているタイマーの上限


	const float KNOCKBACK_DISTANCE = 300.0f;

	const float DARK_METEO_ACTION_POINT = 40.0f;

	//タイマーを0にしない
	const float START_DECIDE_ACTION_TIMER = 1.0f;

}

float SummonerSM_Vigilance::m_decideActionTimer = START_DECIDE_ACTION_TIMER;

SummonerSM_Vigilance::~SummonerSM_Vigilance()
{
}

void SummonerSM_Vigilance::Execute()
{
	//処理を止めているなら処理をしない
	if (m_summoner->IsStopProcessing() == true)
	{
		return;
	}

	if (m_summoner->isAnimationEnable() != true)
	{
		return;
	}

	//行動を決める
	ProcessDecideAction();
}

void SummonerSM_Vigilance::Init(bool saveTimerlesetFlag)
{
	if (saveTimerlesetFlag == true)
	{
		//各種タイマーをリセット
		m_stayPlayerTimer = 0.0f;

		m_decideActionTimer = START_DECIDE_ACTION_TIMER;
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
	//待機タイマーを加算
	m_waitTimer += g_gameTime->GetFrameDeltaTime();

	//ノックバックするか決める、決まったら即ノックバック
	if (IsKnockBack() == true)
	{
		//ノックバックアニメーションステート
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		return;
	}
	


	//追いかけるかその場で待機するか決める
	if (IsChasePlayer() == true)
	{
		//歩きアニメーションステート
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_Walk);
		return;
	}
	else
	{
		//待機アニメーションステート
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_Idle);
		return;
	}
	
}

bool SummonerSM_Vigilance::IsKnockBack()
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

		return true;

	}

	return false;
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

bool SummonerSM_Vigilance::IsChasePlayer()
{
	//一定の距離内にプレイヤーがいるなら確定で待機
	if (m_summoner->IsFindPlayer(400.0f) == true)
	{
		//待機
		return false;
	}

	//一度待機したら一定時間そのまま
	if (m_decideActionTimer < 5.0f)
	{
		//行動決定タイマーを加算
		m_decideActionTimer += g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		//タイマーをリセット
		m_decideActionTimer = 0.0f;

		//確率で待機にする
		int probability = rand() % 10;

		//確率が一定値より大きかったらその場で待機
		if (probability > 4)
		{
			m_idleStateFlag = false;
			//待機
			return m_idleStateFlag;
		}
		else
		{
			m_idleStateFlag = true;
			//移動
			return m_idleStateFlag;
		}
	}
	//ここに来たら前フレームの行動と同じ
	return m_idleStateFlag;
	
}
