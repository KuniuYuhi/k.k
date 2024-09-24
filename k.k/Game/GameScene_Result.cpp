#include "stdafx.h"
#include "GameScene_Result.h"

#include "Result.h"
#include "GameFinishCamera.h"
#include "Game.h"

#include "GameSceneManager.h"


void GameScene_Result::Entry()
{
	//シーン切り替え可能かフラグをリセット
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);

	//勝ったか負けたかによって処理を変える

	switch (GameSceneManager::GetInstance()->GetBattleOutCome())
	{
	case GameSceneManager::enBattleOutCome_PlayerWin:
		//プレイヤーが勝利した！
		PlayerWinProcess();
		break;
	case GameSceneManager::enBattleOutCome_PlayerLose:
		//プレイヤーが敗北した…
		PlayerLoseProcess();
		break;
	default:
		std::abort();
		break;
	}


	//リザルトシーンを生成
	//CreateResult();

	
}

void GameScene_Result::Update()
{
	//リザルトでローディングを生成して
	//全部処理が終わったら

	//
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{

		//シーン切り替え可能かフラグをリセット
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//タイトルシーンに切り替え
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_Title);
		return;
	}


}

void GameScene_Result::Exit()
{
	if (m_gameFinishCamera != nullptr)
	{
		DeleteGO(m_gameFinishCamera);
	}
	
	Game* game = FindGO<Game>("game");
	game->DeleteThis();

	//リザルトシーンを削除
	DeleteGO(m_resultSeen);

	//勝敗をリセットする
	GameSceneManager::GetInstance()->SetBattleOutCome(GameSceneManager::enBattleOutCome_None);

}



void GameScene_Result::PlayerWinProcess()
{
	//ゲームカメラの削除
	Game* game = FindGO<Game>("game");

	game->DeleteGameCamera();

	//ゲームフィニッシュカメラの生成
	m_gameFinishCamera = NewGO<GameFinishCamera>(0, "gamefinishcamera");

	//BGMを小さくしていく

	
}

void GameScene_Result::PlayerLoseProcess()
{
	//BGMを小さくしていく

	CreateResult();
}

void GameScene_Result::CreateResult()
{
	m_resultSeen = NewGO<ResultSeen>(0, "result");


}
