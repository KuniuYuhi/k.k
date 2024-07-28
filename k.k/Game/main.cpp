#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"
#include "SoundFile.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("FantasyBattle"));
	
	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	//カメラの設定
	g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	

	//サウンドファイルの読み込み
	SoundFile* soundFile = new SoundFile();
	delete soundFile;

	//フェードクラスの初期化
	Fade*m_fade = NewGO<Fade>(5, "fade");

	//ゲームマネージャーを生成
	//シーンステートをタイトルステートに設定
	//GameSceneManager::CreateInstanceAndSetGameSceneState(enGameSceneState_Title);

	//Title* title = NewGO<Title>(0, "game");
	Game* game = NewGO<Game>(0, "game");

	// ここからゲームループ。
	while (DispatchWindowMessage() && g_gameLoop.m_isLoop == true)
	{
		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		//GameSceneManager::GetInstance()->Update();

		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	//開放しないとD3D12オブジェクト開放のエラーはなくなる
	delete g_k2EngineLow;

	//GameSceneManager::GetInstance()->DeleteInstance();

	return 0;
}

