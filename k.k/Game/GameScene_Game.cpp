#include "stdafx.h"
#include "GameScene_Game.h"

#include "GameSceneManager.h"

#include "Fade.h"

#include "GameCamera.h"

#include "Game.h"

namespace {
	const float DEFAULT_BATTLE_BGM = 2.0f;

}

void GameScene_Game::Entry()
{
	//フェードアウトする

	Fade* fade = FindGO<Fade>("fade");
	fade->StartFadeOut(3.0f);

	GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
	//ゲーム開始時のカメラの設定
	gameCamera->SetBattleStartCamera();

	//todo ここでゲームクラスを作成


	//ボスを生成
	m_game = FindGO<Game>("game");
	m_game->CreateBoss();
	m_game->CreateGameUI();
	m_game->CreateBattlePhase();

	//BGMの音量を上げる
		//BGMの再生
	g_soundManager->InitAndPlaySoundSource(enSoundName_BattleBGM, DEFAULT_BATTLE_BGM, false, true, true);

}

void GameScene_Game::Update()
{
	//勝敗が着いたらステートを変更
	if (GameSceneManager::GetInstance()->GetBattleOutCome() !=
		GameSceneManager::enBattleOutCome_None)
	{

		g_soundManager->StopSound(enSoundName_BattleBGM);

		//シーン切り替え可能かフラグをリセット
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//リザルトシーンに切り替え
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_Result);
		return;
	}


	//
	m_game->UpdateGameSystem();

}

void GameScene_Game::Exit()
{
	//

}

