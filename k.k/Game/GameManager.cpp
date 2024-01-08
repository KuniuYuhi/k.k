#include "stdafx.h"
#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "BattleStart.h"
#include "Player.h"
#include "Boss.h"

namespace {
	const float SECOND_MAX = 60.0f;

	const float PHASE_TIME_LIMMIT = 50.0f;
	const float BREAK_TIME_LIMMIT = 10.0f;
	const float SHORT_BREAK_LIMMIT = 5.0f;

	const Vector3 BOSS_START_POSITON = { 0.0f, 0.0f, 600.0f };

	const Vector3 PLAYER_START_POSITON = { 0.0f, 0.0f, -400.0f };
}

GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager(EnGameSeenState startGameSeenState)
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;
	//ゲーム開始時のステートを設定
	m_enGameSeenState = startGameSeenState;
}

GameManager::~GameManager()
{
	//キャラ情報マネージャー削除
	CharactersInfoManager::DeleteInstance();

	m_instance = nullptr;
}

void GameManager::Init()
{
	//キャラクターの情報マネージャーの生成
	CharactersInfoManager::CreateInstance();
}

void GameManager::CreateBattleStartClass()
{
	//バトルスタートクラス生成
	BattleStart* battleStart = NewGO<BattleStart>(0, "battlestart");
}

void GameManager::CreatePlayerClass()
{
	Player* player = NewGO<Player>(0, "player");
	//初期座標の設定
	player->SetPosition(PLAYER_START_POSITON);
	CharactersInfoManager::GetInstance()->SetPlayerInstance(player);
}

void GameManager::DeletePlayerClass()
{
	CharactersInfoManager::GetInstance()->DeletePlayerInstance();
}

void GameManager::CreateBoss()
{
	Boss* boss = NewGO<Boss>(0, "boss");
	//初期座標の設定
	boss->SetPosition(BOSS_START_POSITON);
	CharactersInfoManager::GetInstance()->SetBossInstance(boss);
}

void GameManager::DeleteBossClass()
{
	CharactersInfoManager::GetInstance()->DeleteBossInstance();
}

void GameManager::Execute()
{
	//勝敗が着いたら処理しない
	if (GetOutComeState() != enOutComeState_None)
	{
		return;
	}
	//制限時間の計算
	CalcTimeLimmit();
	//フェーズの処理
	ProcessPhase();
	//勝敗が着いたか
	IsOutComeDecided();
}

void GameManager::ProcessPhase()
{
	switch (m_enPhaseState)
	{
	case GameManager::EnPhaseState_Phase1:
		OnProcessPhaseTransition();
		break;
	case GameManager::EnPhaseState_Phase2:
		OnProcessPhaseTransition();
		break;
	case GameManager::EnPhaseState_Phase3:
		OnProcessPhaseTransition();
		break;
	case GameManager::EnPhaseState_BreakTime:
		OnProcessBreakTimeTransition();
		break;
	default:
		break;
	}
}

void GameManager::OnProcessPhaseTransition()
{
	switch (m_enPhaseStep)
	{
		//フェーズの制限時間の計算
	case GameManager::enPhaseStep_PhaseTime:
		CalcPhaseTime();
		break;
		//小休憩の時間の計算
	case GameManager::enPhaseStep_ShortBreak:
		ProcessShortBreak();
		break;
	default:
		break;
	}
}

void GameManager::CalcPhaseTime()
{
	//一つのフェーズの時間が終わったら
	if (m_phaseTimer > PHASE_TIME_LIMMIT)
	{
		//フェーズタイマーをリセット
		m_phaseTimer = 0.0f;
		//フェーズが終わったので、モンスターを削除するようにする
		SetDAndCMonstersState(enMonsters_Delete);
		//次のフェーズが休憩時間なら、ステップを進めない
		if ((m_enPhaseState + 1) == EnPhaseState_BreakTime)
		{
			//休憩時間フェーズに進む
			m_enPhaseState = static_cast<EnPhaseState>(m_enPhaseState + 1);
			return;
		}
		//次のステップに進む
		m_enPhaseStep = enPhaseStep_ShortBreak;
		
	}
	else
	{
		//タイマーを加算
		m_phaseTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void GameManager::ProcessShortBreak()
{
	if (m_shortBreakTimer > SHORT_BREAK_LIMMIT)
	{
		//小休憩タイマーをリセット
		m_shortBreakTimer = 0.0f;
		//次のフェーズに進む
		m_enPhaseState = static_cast<EnPhaseState>(m_enPhaseState + 1);
		//次のフェーズが休憩時間でないなら、モンスター召喚
		if (m_enPhaseState != EnPhaseState_BreakTime)
		{
			//フェーズが切り替わったので、モンスターを生成する
			SetDAndCMonstersState(enMonsters_Create);
		}
		//ステップをリセット
		m_enPhaseStep = enPhaseStep_PhaseTime;
	}
	else
	{
		//タイマーを加算
		m_shortBreakTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void GameManager::OnProcessBreakTimeTransition()
{
	//一定時間経ったら休憩終了
	if (m_breakTimeTimer > BREAK_TIME_LIMMIT)
	{
		m_breakTimeTimer = 0.0f;
		//フェーズを一番最初に戻す
		m_enPhaseState = EnPhaseState_Phase1;
		//フェーズが切り替わったので、モンスターを生成する
		SetDAndCMonstersState(enMonsters_Create);
		return;
	}
	else
	{
		//タイマーを加算
		m_breakTimeTimer += g_gameTime->GetFrameDeltaTime();
	}

}

void GameManager::CalcTimeLimmit()
{
	//0秒以下なら
	if (m_second <= 0) {
		//1分減らす
		m_minute--;
		//もし0分なら、秒も0にする
		if (m_minute < 0) {
			m_second = 0.0f;
			m_minute = 0.0f;
			//制限時間に達したのでプレイヤーの負け
			m_playerLoseFlag = true;
		}
		//60秒に戻す
		else
		{
			m_second = SECOND_MAX;
		}
	}
	else
	{
		//秒を減らす
		m_second -= g_gameTime->GetFrameDeltaTime();
	}
}

void GameManager::IsOutComeDecided()
{
	//負け
	if (m_playerLoseFlag == true ||
		CharactersInfoManager::GetInstance()
		->GetPlayerInstance()->IsDeadPlayer() == true)
	{
		SetOutComeState(enOutComeState_PlayerLose);
	}
	//勝ち
	if (m_playerWinFlag == true)
	{
		SetOutComeState(enOutComeState_PlayerWin);
	}
}






