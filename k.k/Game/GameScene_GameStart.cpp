#include "stdafx.h"
#include "GameScene_GameStart.h"

#include "GameSceneManager.h"

#include "Game.h"
#include "BattleStart.h"


void GameScene_GameStart::Entry()
{
	//シーン切り替え可能かフラグをリセット
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);


	Game* game = NewGO<Game>(0, "game");

	//バトルスタートクラス作成
	m_battleStart = NewGO<BattleStart>(0, "battlestart");
}

void GameScene_GameStart::Update()
{
	//バトルスタートの処理が終わったら次のステートに遷移
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{
		//シーン切り替え可能かフラグをリセット
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//ボス登場シーンに切り替え
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_AppearanceBoss);
		return;
	}

}

void GameScene_GameStart::Exit()
{
	//バトルスタートクラスを削除
	DeleteGO(m_battleStart);
}
