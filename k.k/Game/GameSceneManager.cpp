#include "stdafx.h"
#include "GameSceneManager.h"

#include "GameSceneContext.h"



GameSceneManager* GameSceneManager::m_instance = nullptr;


namespace {

	const float TIME_LIMIT = 5.0f;		//制限時間
	const float SECONT_MAX = 60.0f;
}

GameSceneManager::GameSceneManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;

}

GameSceneManager::~GameSceneManager()
{
	m_instance = nullptr;
}

void GameSceneManager::CreateInstanceAndSetGameSceneState(EnGameSceneState setGameSceneState)
{
	//ゲームシーンを生成
	m_instance = new GameSceneManager();
	//ゲームシーンコンテキストを生成
	m_instance->CreateGameSceneContext(setGameSceneState);

	
}

void GameSceneManager::CreateGameSceneContext(EnGameSceneState setGameSceneState)
{
	m_gameSceneContext = std::make_unique<GameSceneContext>();
	//初期化処理で管理するステートを設定
	m_gameSceneContext.get()->Init(setGameSceneState);
}

void GameSceneManager::Update()
{
	if (m_gameSceneContext == nullptr)
	{
		return;
	}
	//現在のシーンの更新処理を実行
	m_gameSceneContext.get()->UpdateGameSceneState();

	//タイマーの更新
	UpdateGameTimer();
}

void GameSceneManager::ChangeGameSceneState(EnGameSceneState cangeGameSceneState)
{
	m_gameSceneContext.get()->ChangeState(cangeGameSceneState);
}

const EnGameSceneState& GameSceneManager::GetCurrentGameSceneState()
{
	return m_gameSceneContext.get()->GetCurrentGameSceneState();
}



void GameSceneManager::StartGameTimer()
{
	m_isStartGameTimer = true;
	//制限時間を設定
	m_gameTimer.SetMinute(TIME_LIMIT);

	m_isTimeOver = false;
}

void GameSceneManager::UpdateGameTimer()
{
	//スタートしていないなら処理しない
	if (!m_isStartGameTimer) return;

	//時間切れなら処理しない
	if (m_isTimeOver) return;

	CalcGameTimer();

}

void GameSceneManager::CalcGameTimer()
{
	//0秒以下なら
	if (m_gameTimer.GetSecond() <= 0) {
		//1分減らす
		m_gameTimer.SubMinute(1.0f);
		//もし0分なら、秒も0にする
		if (m_gameTimer.GetMinute() < 0) {
			m_gameTimer.SetSecond(0.0f);
			m_gameTimer.SetMinute(0.0f);
			
			//時間切れ！！！
			m_isTimeOver = true;
			//勝敗を設定（プレイヤーの負け！！）
			SetOutComPlayerLose();
		}
		//60秒に戻す
		else
		{
			m_gameTimer.SetSecond(SECONT_MAX);
		}
	}
	else
	{
		//秒を減らす
		m_gameTimer.SubSecond(g_gameTime->GetFrameDeltaTime());
	}


}
