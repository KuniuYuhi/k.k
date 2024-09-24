#include "stdafx.h"
#include "SummonerSSM_Offensive.h"
#include "Summoner.h"
#include "SummonerSMContext.h"

#include "EnemyManager.h"

#include "Brave.h"


namespace {
	const float DARKMETEORITE_COOLTIME_LIMIT = 3.0f;

	const float MELEE_ATTACK_DISTANCE = 400.0f;
}

void SummonerSSM_Offensive::Entry()
{
	if (m_player == nullptr)
	{
		m_player = FindGO<Brave>("Brave");
	}

}

void SummonerSSM_Offensive::Update()
{
	//アクション中は処理しない
	if (m_summoner->IsAction())
	{
		return;
	}

		
	SelectAttackBasedOnDistance();
	


}

bool SummonerSSM_Offensive::IsEnableChangeStateMachine()
{
	//予約ステートをなしにする
	m_stateMachineContext->SetReservationState(enSummonerState_None);

	//もし条件がそろえば普段のステートマシンに遷移

	//HPがへったから離れたい
	//ワープの予約ステートも設定しておく

	//コンボ攻撃なら次のコンボ攻撃に行く
	//次のアクションが決まったのでステートマシンは切り替えない
	if (ChangeNextComboAttackState())
	{
		return false;
	}

	if (CheckRangedAttack())
	{
		return false;
	}
	


	//次の攻撃に行く前にタイマーを入れる

	m_stateMachineContext->ChangeSubStateMachine(enSubStateMachine_General);
	return true;
}

void SummonerSSM_Offensive::SelectAttackBasedOnDistance()
{
	//予約ステートがあるなら
	if (m_stateMachineContext->IsReservationState())
	{
		//予約されているステートに遷移
		m_summoner->ChangeState(m_stateMachineContext->GetReservationState());
		m_stateMachineContext->SetPreviousState(m_stateMachineContext->GetReservationState());
		return;
	}

	//自身からプレイヤーに向かうベクトルから距離を取得
	float distance = m_summoner->GetDistanceToPlayerPositionValue();


	if (distance <= MELEE_ATTACK_DISTANCE)
	{
		SelectMeleeAttack();
	}
	else
	{
		SelectRangedAttack();
	}
}

void SummonerSSM_Offensive::SelectMeleeAttack()
{
	//確率でどちらにするか選ぶ

	int r = rand() % 10;
	//

	if (r > 4)
	{
		//コンボ攻撃の一コンボ目
		m_summoner->ChangeState(enSummonerState_FirstAttack);
		//前回のステートを設定
		m_stateMachineContext->SetPreviousState(enSummonerState_FirstAttack);
	}
	else
	{

		//ダークメテオに切り替えるかチェック
		if (CheckChangeDarkMeteoriteState())
		{
			m_summoner->ChangeState(enSummonerState_Attack_DarkMeteorite);
			//各種要素を計算
			m_darkMeteoriteRemainingCount--;
			m_darkMeteoriteCoolTimer = 0.0f;
			//前回のステートを設定
			m_stateMachineContext->SetPreviousState(enSummonerState_Attack_DarkMeteorite);
			return;
		}

		//範囲攻撃
		m_summoner->ChangeState(enSummonerState_ShockWave);
		//前回のステートを設定
		m_stateMachineContext->SetPreviousState(enSummonerState_ShockWave);
	}
	


	

}

void SummonerSSM_Offensive::SelectRangedAttack()
{
	//切り替えステート
	EnSummonerState changeState = enSummonerState_None;

	//遠くにいるのにダメージを受けている＝遠距離攻撃
	//強制的に近くに飛んでいく


	//ダークメテオに切り替えるかチェック
	if (CheckChangeDarkMeteoriteState())
	{
		changeState = enSummonerState_Attack_DarkMeteorite;
		//各種要素を計算
		m_darkMeteoriteRemainingCount--;
		m_darkMeteoriteCoolTimer = 0.0f;
	}
	//ダークボールに切り替えるかチェック
	else if (CheckChangeDarkBallState())
	{
		changeState = enSummonerState_DarkBall;
	}

	//changeState = enSummonerState_Attack_DarkMeteorite;
	////各種要素を計算
	//m_darkMeteoriteRemainingCount--;
	//m_darkMeteoriteCoolTimer = 0.0f;

	//ステートが切り替わらないなら処理しない
	if (changeState == enSummonerState_None) return;

	//ステート切り替え
	m_summoner->ChangeState(changeState);
	//前回のステートを設定
	m_stateMachineContext->SetPreviousState(changeState);
}

bool SummonerSSM_Offensive::ChangeNextComboAttackState()
{
	//前回のステートがコンボ攻撃ステートなら
	if (m_stateMachineContext->GetPreviousState() == enSummonerState_FirstAttack ||
		m_stateMachineContext->GetPreviousState() == enSummonerState_SecondAttack)
	{
		//次のコンボ攻撃ステートにする
		EnSummonerState state = 
			static_cast<EnSummonerState>(m_stateMachineContext->GetPreviousState() + 1);
		//予約ステート設定
		m_stateMachineContext->SetReservationState(state);

		return true;
	}

	return false;
}

bool SummonerSSM_Offensive::CheckRangedAttack()
{
	if (CheckNextDarkBallAction())
	{
		//次のアクションをダークボールに決定
		//予約ステート設定
		m_stateMachineContext->SetReservationState(enSummonerState_DarkBall);


		return true;
	}

	return false;
}

bool SummonerSSM_Offensive::CheckNextDarkBallAction()
{
	//スコア
	int score = 0;
	//前回のステートがダークボールステートなら確率を下げる。
	//連続で使用するたび確率を下げる
	if (m_stateMachineContext->GetPreviousState() == enSummonerState_DarkBall)
	{
		score -= 15 * m_darkBallActionCount;
	}

	//体力が減っていくと連続で使う頻度が上がる
	//HPの割合を求める
	float ratio = 
		(float)m_summoner->GetCommonStatus().GetCurrentHp() / (float)m_summoner->GetCommonStatus().GetMaxHp();
	//割合が7割以下ならスコアを加算する
	if (ratio <= 0.7f)
	{
		//残っているHPの割合を減ったHPの割合にする
		ratio = 1 - ratio;

		ratio = ratio * 10;

		int addProbability = rand() % 10 + 1;

		//スコアを加算する
		score += ratio * addProbability;

	}



	//スコアが一定以上ならこの行動にする
	if (score > 30) {
		//ダークボールカウント加算
		m_darkBallActionCount++;
		return true;
	}

	//この中の行動をしなかったのでカウントをリセット
	m_darkBallActionCount = 0;
	return false;
}

bool SummonerSSM_Offensive::CheckChangeDarkBallState()
{
	//無条件でtrue
	return true;
}

bool SummonerSSM_Offensive::CheckChangeDarkMeteoriteState()
{
	//プレイヤーのスタミナを見る
	//プレイヤーの周りに敵がいるか
	//HPの割合に分けてその回数だけ使えるようにする


	//クールタイムが終わっていないなら処理しない
	if (m_darkMeteoriteCoolTimer < DARKMETEORITE_COOLTIME_LIMIT)
	{
		//タイマーを加算して処理を終わる
		m_darkMeteoriteCoolTimer ++;
		return false;
	}

	//スコア
	int score = 0;

	//現在のHPを最大HPを割った数で割って実質の残り回数を求める
	int result = m_summoner->GetCommonStatus().GetCurrentHp() / m_stateMachineContext->GetMaxHpQuotient();
	//結果と残り回数が同じなら使用できる状態
	if (result <= m_darkMeteoriteRemainingCount)
	{
		score += 25;
	}

	//現在のスタミナを最大値スタミナで割って割合を計算
	float staminaRatio = m_player->GetStatus().GetCurrentStamina() / m_player->GetStatus().GetMaxStamina();
	//逆にしてスタミナが少ないほどレートが上がるようにする
	staminaRatio = 1 - staminaRatio;
	//割合を使ってスコア加算。スタミナが少ないほどスコアが大きくなる
	score += 100 * (staminaRatio * 1.2f);

	result = EnemyManager::GetInstance()->GetNearbyEnemyCount(m_player->GetPosition(), 350.0f);
	//5体以上いたら5に制限する
	if (result > 6) result = 6;

	score += result * 14;

	//最終的なスコアが120以下ならこの行動はしない
	if (score < 88) return false;

	return true;
}
