#include "stdafx.h"
#include "IBossStateMachine.h"
#include "Summoner.h"
#include "ISummonerState.h"

#include "CharactersInfoManager.h"


//todo ProcessMeleeAttack(),ProcessLongRangeAttack()の引数にオーダーをいれる

namespace {
	const float WAIT_TIME = 5.0f;		//待機時間


	const float STAY_PLAYER_LIMMIT_TIME = 6.0f;		//プレイヤーが近くにとどまっているタイマーの上限
}

IBossStateMachine* IBossStateMachine::m_stateMachineInstance = nullptr;

IBossStateMachine::IBossStateMachine(Summoner* bossInstance)
{
	m_summoner = bossInstance;
}

IBossStateMachine::~IBossStateMachine()
{
	m_stateMachineInstance = nullptr;
}

void IBossStateMachine::Execute()
{


	//行動を決める
	ProcessDecideAction();

}

void IBossStateMachine::ProcessDecideAction()
{
	//どんな攻撃をするか決める
	//		攻撃			待機
	//		｜
	//		｜
	//近距離　遠距離
	//	｜		｜
	//

	//一定間隔で攻撃。ある条件で待機状態でも攻撃に移行
	//サモナーが攻撃中は処理しない
	if (m_summoner->IsAttackEnable() == false)
	{
		return;
	}
	//これから行動を決める
	DecideNextAction();
	//行動によって連続でアクションするか決める
}

void IBossStateMachine::DecideNextAction()
{
	//連続攻撃するなら抽選する

	//まず待機ステートか攻撃ステートか決める
	//待機時間の間またはもう一度攻撃するチャンスがないなら
	if (m_waitTimer < WAIT_TIME&&m_lotteryChanceFlag == false)
	{
		//待機ステートを決める
		ProcessWaitState();
	}
	else
	{
		//攻撃ステートを決める
		ProcessAttackState();
	}
}

void IBossStateMachine::ProcessAttackState()
{
	//どんな攻撃にするか決める

	//サモナーから勇者に向かうベクトルを計算する
	m_toPlayer = m_summoner->GetPosition() -
		CharactersInfoManager::GetInstance()->GetBraveInstance()->GetPosition();

	//距離によって近距離攻撃か遠距離攻撃か決める
	if (m_toPlayer.Length() > 200.0f)
	{
		//遠距離攻撃
		ProcessLongRangeAttack();
	}
	else
	{
		//近距離攻撃
		ProcessMeleeAttack();
	}

	//行動した後の処理
	m_waitTimer = 0.0f;
	//起こしたアクションを記憶
	m_oldActionName = m_summoner->GetNowSummonerState()->GetActionName();
}

void IBossStateMachine::ProcessWaitState()
{
	//タイマーを加算
	m_waitTimer += g_gameTime->GetFrameDeltaTime();


	//長い間勇者が近くにいるなら
	//逃げるかノックバックで引き離す
	if (m_toPlayer.Length() < 200.0f)
	{
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
		//強制的に攻撃ステート決めに移る
		//近距離攻撃
		ProcessMeleeAttack();
		//とどまっているタイマーリセット
		m_stayPlayerTimer = 0.0f;
		//waitタイマーリセットする？
	}



	//前の行動がダークボールなら
	//if (m_oldActionName == enActionName_Darkball&& m_lotteryChance>0)
	//{
	//	SetRandomActionValue();
	//	m_lotteryChance--;
	//}

	////もう一度行動する度に確率を下げる
	//if (m_randomValue > attackRetryProbability)
	//{
	//	//もう一度行動するので待機時間の短縮
	//	m_waitTimer = WAIT_TIME;
	//	DecideNextAction();
	//	return;
	//}
}

void IBossStateMachine::ProcessLongRangeAttack()
{
	

	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_DarkBall);

	int chance = rand() % 10;
	if (chance > attackRetryProbability && m_lotteryChanceFlag == false)
	{
		//もう一度攻撃するチャンスあり
		m_lotteryChanceFlag = true;
		/*attackRetryProbability *= 2;
		if (attackRetryProbability > 10)
			attackRetryProbability = 10;*/
		return;
	}

	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}
}

void IBossStateMachine::ProcessMeleeAttack()
{
	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}

	//近距離攻撃カウントを増やす
	m_meleeCount++;

	//自身の周囲にモブがいない、長い間近くにいるならノックバック
	if (m_stayPlayerTimer > STAY_PLAYER_LIMMIT_TIME)
	{
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		return;
	}

	int bb = rand() % 10;

	if (bb > 4)
	{
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkWall);
	}
	else
	{
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
	}
	
}

void IBossStateMachine::AddStayPlayerTimer()
{
	//タイマーを加算
	m_stayPlayerTimer += g_gameTime->GetFrameDeltaTime() * m_accelerateStayPlayerTimer;
	//何らかの要因でさらにタイマーを加速-＞m_accelerateStayPlayerTimer
	




}
