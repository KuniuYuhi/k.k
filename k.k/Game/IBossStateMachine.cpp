#include "stdafx.h"
#include "IBossStateMachine.h"
#include "Summoner.h"
#include "ISummonerState.h"

#include "Player.h"
#include "CharactersInfoManager.h"


//todo ProcessMeleeAttack(),ProcessLongRangeAttack()の引数にオーダーをいれる

namespace {
	const float WAIT_TIME = 2.0f;		//待機時間

	const float MELEE_ATTACK_RANGE = 280.0f;	//近距離攻撃の範囲内

	const float STAY_PLAYER_LIMMIT_TIME = 10.0f;		//プレイヤーが近くにとどまっているタイマーの上限


	const float KNOCKBACK_DISTANCE = 200.0f;

	const float DARK_METEO_ACTION_POINT = 1.0f;

}

IBossStateMachine* IBossStateMachine::m_stateMachineInstance = nullptr;

IBossStateMachine::IBossStateMachine(Summoner* bossInstance)
{
	m_summoner = bossInstance;

	//乱数を初期化。
	srand((unsigned)time(NULL));
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

	//怒りモードカウント加算
	//m_angryCount++;

	//行動した後の処理
	m_waitTimer = 0.0f;
	//起こしたアクションを記憶
	m_oldActionName = m_summoner->GetNowSummonerState()->GetActionName();
}

void IBossStateMachine::ProcessWaitState()
{
	//タイマーを加算
	m_waitTimer += g_gameTime->GetFrameDeltaTime();

	//怒りモード
	//if (m_summoner->GetNowSpecialActionState() !=
	//	Summoner::enSpecialActionState_AngryMode)
	//{
	//	if (m_angryCount > 3)
	//	{
	//		m_summoner->
	//			SetNextAnimationState(Summoner::enAnimationState_Angry);
	//		//タイマーリセット
	//		m_angryCount = 0;
	//		return;
	//	}
	//}
	
	//ノックバックするか決める、決まったら即ノックバック
	ProcessIsKnockBack();

}

void IBossStateMachine::ProcessLongRangeAttack()
{
	//遠距離攻撃

	//ダークメテオ
	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_Attack_DarkMeteorite_start);
	return;

	//前の行動がダークメテオでないなら
	//ダークメテオを撃つか調べる
	if (IsNextActionDarkMeteo() == true&& m_oldActionName != enActionName_DarkMeteo_Start)
	{
		//ダークメテオ
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_Attack_DarkMeteorite_start);
		return;
	}

	//ダークボール
	ProcessPlayAnimDarkBall();

	//二回以上連続攻撃はしない
	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}
}

void IBossStateMachine::ProcessMeleeAttack()
{
	//近距離攻撃

	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}

	//近距離攻撃カウントを増やす
	m_meleeCount++;

	//自身の周囲にモブがいない、長い間近くにいるならノックバック
	if (m_stayPlayerTimer > STAY_PLAYER_LIMMIT_TIME)
	{
		//ノックバック
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		//ノックバック加速タイマーリセット
		m_accelerateStayPlayerTimer = 1.0f;
		return;
	}

	int bb = rand() % 10;
	//確率で変化
	if (bb > 4)
	{
		//ダークウォール
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkWall);
	}
	else
	{
		//通常攻撃
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
	}
	
}

void IBossStateMachine::AddStayPlayerTimer()
{
	//条件によってタイマーを加速させる
	IsKnockBackTimerAccelerate();
	//タイマーを加算
	m_stayPlayerTimer += g_gameTime->GetFrameDeltaTime() * m_accelerateStayPlayerTimer;
	//何らかの要因でさらにタイマーを加速-＞m_accelerateStayPlayerTimer
}

void IBossStateMachine::ProcessPlayAnimDarkBall()
{
	//二連続で撃ったり、追尾するダークボールにしたりする
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

	//二回以上連続攻撃はしない
	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}
}

bool IBossStateMachine::IsNextActionDarkMeteo()
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
	return false;
}

void IBossStateMachine::ProcessIsKnockBack()
{
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
		//強制的に攻撃ステート決めに移る
		//近距離攻撃
		ProcessMeleeAttack();
		//とどまっているタイマーリセット
		m_stayPlayerTimer = 0.0f;
		//waitタイマーリセットする？
		//ノックバックした後の攻撃は一秒は空ける
		if (m_waitTimer > 1.0f)
		{
			m_waitTimer = 1.0f;
		}
	}
}

void IBossStateMachine::IsKnockBackTimerAccelerate()
{

	//if()
}
