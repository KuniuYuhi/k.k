#include "stdafx.h"
#include "GameSceneManager.h"

#include "GameSceneContext.h"



GameSceneManager* GameSceneManager::m_instance = nullptr;

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
}

void GameSceneManager::ChangeGameSceneState(EnGameSceneState cangeGameSceneState)
{
	m_gameSceneContext.get()->ChangeState(cangeGameSceneState);
}

const EnGameSceneState& GameSceneManager::GetCurrentGameSceneState()
{
	return m_gameSceneContext.get()->GetCurrentGameSceneState();
}
