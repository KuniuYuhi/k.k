#include "stdafx.h"
#include "GameScene_Title.h"

#include "GameSceneManager.h"

#include "Title.h"

#include "Fade.h"


void GameScene_Title::Entry()
{
	//シーン切り替え可能かフラグをリセット
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);

	Title* m_title = NewGO<Title>(0, "title");

	Fade* fade = FindGO<Fade>("fade");
	fade->StartFadeOut(5.0f);

}

void GameScene_Title::Update()
{
	//バトルスタートの処理が終わったら次のステートに遷移
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{
		//シーン切り替え可能かフラグをリセット
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//ボス登場シーンに切り替え
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_GameStart);
		return;
	}
}

void GameScene_Title::Exit()
{
}
