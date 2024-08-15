#include "stdafx.h"
#include "GameScene_AppearanceBoss.h"

#include "EntryBoss.h"
#include "Game.h"

#include "GameSceneManager.h"



void GameScene_AppearanceBoss::Entry()
{
	//ボス登場クラス生成
	m_entryBoss = NewGO<EntryBoss>(0, "entryboss");
	
	Game* game = FindGO<Game>("game");
	game->CreatePlayerAndCamera();

	

}

void GameScene_AppearanceBoss::Update()
{
	//時間経過かスキップされたら次のステートに切り替える
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{
		//シーン切り替え可能かフラグをリセット
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//ゲームシーンに切り替え
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_Game);
		return;
	}

}

void GameScene_AppearanceBoss::Exit()
{
	//ボス登場クラス削除
	DeleteGO(m_entryBoss);
}
