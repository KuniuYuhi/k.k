#include "stdafx.h"
#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "BattleStart.h"
#include "Player.h"
#include "Lich.h"

namespace {
	const float SECOND_MAX = 60.0f;
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

void GameManager::Execute()
{


	CalcTimeLimmit();
}

void GameManager::CreateBattleStartClass()
{
	//バトルスタートクラス生成
	BattleStart* battleStart = NewGO<BattleStart>(0, "battlestart");
}

void GameManager::CreatePlayerClass()
{
	Player* player = NewGO<Player>(0, "player");
	CharactersInfoManager::GetInstance()->SetPlayerInstance(player);
}

void GameManager::CreateBoss()
{
	Lich* lich = NewGO<Lich>(0, "lich");
	CharactersInfoManager::GetInstance()->SetLichInstance(lich);
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
			//制限時間に達した
			//ゲームオーバーステートを設定
			SetGameSeenState(enGameSeenState_GameOver);
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




