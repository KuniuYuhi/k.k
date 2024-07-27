#include "stdafx.h"
#include "GameSceneContext.h"


#include "IGameSceneState.h"

#include "GameScene_Title.h"
#include "GameScene_GameStart.h"
#include "GameScene_AppearanceBoss.h"
#include "GameScene_Game.h"
#include "GameScene_Pause.h"
#include "GameScene_Result.h"

void GameSceneContext::Init(EnGameSceneState startGameSceneState)
{

	//ステートを切り替える
	ChangeState(startGameSceneState);

}

void GameSceneContext::UpdateGameSceneState()
{
	if (m_currentGameScene == nullptr)
	{
		return;
	}

	m_currentGameScene->Update();
}

void GameSceneContext::ChangeState(EnGameSceneState changeGameSceneState)
{
	//中身があるなら削除
	if (m_currentGameScene != nullptr)
	{
		//削除前の処理
		m_currentGameScene->Exit();
		//削除
		delete m_currentGameScene;
	}

	//ステートを生成する
	switch (changeGameSceneState)
	{
	case enGameSceneState_Title:
		m_currentGameScene = new GameScene_Title();
		break;
	case enGameSceneState_GameStart:
		m_currentGameScene = new GameScene_GameStart();
		break;
	case enGameSceneState_AppearanceBoss:
		m_currentGameScene = new GameScene_AppearanceBoss();
		break;
	case enGameSceneState_Game:
		m_currentGameScene = new GameScene_Game();
		break;
	case enGameSceneState_Pause:
		m_currentGameScene = new GameScene_Pause();
		break;
	case enGameSceneState_Result:
		m_currentGameScene = new GameScene_Result();
		break;
	default:
		std::abort();
		break;
	}

	//前のステートを保存
	m_previousGameSceneState = m_currentGameSceneState;
	//現在のステートを引数のステートにする
	m_currentGameSceneState = changeGameSceneState;

	//ステート遷移時の最初の処理
	m_currentGameScene->Entry();

}
