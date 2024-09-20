#include "stdafx.h"
#include "SummonerSSM_General.h"
#include "SummonerSMContext.h"
#include "Summoner.h"

#include "EnemyManager.h"

void SummonerSSM_General::Entry()
{
	//サモナーのHPを保存
	m_summonerHp = m_summoner->GetCommonStatus().GetCurrentHp();
}

void SummonerSSM_General::Update()
{
	if (m_summoner->IsAction())
	{
		return;
	}
	

	//状況によってステートを切り替える
	//ステートを切り替えた場合は先の処理をしない
	if (EvaluateSituation()) return;


	//攻撃ステートマシンに切り替えるかチェック
	CheckEnableChangeAttackStateMachine();


	//移動方法をチェック
	CheckMovePattern();


}

void SummonerSSM_General::Exit()
{
}

void SummonerSSM_General::CalcStateMachineChangeTimer()
{
	if (m_stateMachineChangeTimer >= m_stateMachineChangeTimeLimit)
	{
		//ステートマシン切り替え可能
		m_isEnableChangeStateMachine = true;
		return;
	}
	//タイマーを加算
	m_stateMachineChangeTimer += g_gameTime->GetFrameDeltaTime();
}

void SummonerSSM_General::CheckMovePattern()
{
	//移動するのをやめる

	//タイマーの計算
	CalcMovePatternCheckTime();

	//フラグが立ったなら
	if (m_isMovePatternCheck)
	{
		int ram = rand() % 10;

		//プレイヤーとの距離が遠いほど止まる確率が上がる

		float distance = m_summoner->GetDistanceToPlayerPositionValue();
		//線形補間で距離を加算する確率に変換
		//加算する確率は最大3。距離の補間は0～1000
		int add = Math::Lerp((distance - 0.0f) / (1000.0f - 0.0f), 0.0f, 3.0f);

		if (ram < (5 + add))
		{
			//移動を止める
			m_summoner->SetStopMoveFlag(true);
		}
		else
		{
			//移動を止めない
			m_summoner->SetStopMoveFlag(false);
		}

		//フラグリセット
		m_isMovePatternCheck = false;
		//タイマーのリセット
		m_movePatternCheckTimer = 0.0f;
	}
}

void SummonerSSM_General::CalcMovePatternCheckTime()
{
	if (m_movePatternCheckTimer >= m_movePatternCheckTimeLimit)
	{
		//チェックできる
		m_isMovePatternCheck = true;
		return;
	}
	m_movePatternCheckTimer += g_gameTime->GetFrameDeltaTime();
}

void SummonerSSM_General::CheckEnableChangeAttackStateMachine()
{
	if (m_summoner->IsAction())
	{
		return;
	}

	//ステートマシン切り替えタイマーを計算
	CalcStateMachineChangeTimer();

	//一定時間たったら攻撃ステートマシンに切り替える
	if (m_isEnableChangeStateMachine)
	{
		//フラグをリセット
		m_isEnableChangeStateMachine = false;
		//タイマーをリセット
		m_stateMachineChangeTimer = 0.0f;
		//攻撃ステートマシンに切り替え
		m_stateMachineContext->ChangeSubStateMachine(enSubStateMachine_Offensive);
		return;
	}
}

bool SummonerSSM_General::EvaluateSituation()
{
	//各種クールタイム用タイマー
	AddTimer(m_warpCoolTimer);

	//切り替えステート
	EnSummonerState changeState = enSummonerState_None;

	//ワープステートに切り替えるかチェック
	if (CheckChangeWarpState())
	{
		changeState = enSummonerState_Warp;
	}



	//切り替えたいステートがあるなら
	if (changeState != enSummonerState_None)
	{
		//ステートを切り替える
		m_summoner->ChangeState(changeState);
		//前回のステートを設定
		m_stateMachineContext->SetPreviousState(changeState);

		return true;
	}


	return false;

}

bool SummonerSSM_General::CheckChangeWarpState()
{
	//前回もワープしたならワープしない
	if (m_stateMachineContext->GetPreviousState() == enSummonerState_Warp) return false;
	//タイマーが一定時間経っていないならワープしない
	if (m_warpCoolTimer < m_warpCoolTimeLimit) return false;

	//スコア
	int score = 0;

	//HPの割合を計算
	float ratio = (float)m_summoner->GetCommonStatus().GetCurrentHp() / (float)m_summonerHp;
	//1.5割以上削られたら
	ratio = 1 - ratio;
	if (ratio > 0.13f)
	{
		//割合を使ってスコア加算。減ったHP割合が低いほどスコアが大きくなる
		score += 200 * ratio;
	}

	//既に距離が一定以上離れているならワープしない
	float length = m_summoner->GetDistanceToPlayerPositionValue();
	//距離が近いならスコアを加算
	if (length < 700.0f)
	{
		score += 20;
	}

	//ボスの特定の半径の中にいるモブエネミーの数を取得
	int mobEnemyAmount = EnemyManager::GetInstance()->GetNearbyEnemyCount(
		m_summoner->GetPosition(),600.0f
	);

	score += mobEnemyAmount * 3;


	//合計の重みが一定以上なら

	if (score < 60) return false;

	//タイマーをリセット
	m_warpCoolTimer = 0.0f;
	return true;
}








void SummonerSSM_General::AddTimer(float& timer)
{
	timer += g_gameTime->GetFrameDeltaTime();
}
