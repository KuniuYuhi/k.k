#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "BossStage1.h"
#include "Result.h"
#include "GameUI.h"
#include "Fade.h"
#include "EntryBoss.h"
#include "BattleStart.h"
#include "Pause.h"
#include "Title.h"

#include "SkyCube.h"
#include "InitEffect.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "Boss.h"

#include "GameFinishCamera.h"
#include "BattlePhase.h"

#include "Loading.h"

#include "Slime.h"
#include "Cactus.h"
#include "Mushroom.h"
#include "TurtleShell.h"

//todo リッチが消える時に送られる勝敗がいじってないのに変わっている

namespace {

	//スカイキューブの初期の明るさ
	const float START_SKY_CUBE_LMINANCE = 1.02f;

	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.1f, 0.1f, 0.1f);

	const Vector3 SPOT_LIGHT_COLOR = Vector3(40.0f, 10.0f, 10.0f);

	//ボスとプレイヤーの生成する座標
	const Vector3 BOSS_CREATE_POSITION = Vector3(0.0f, 0.0f, 600.0f);
	const Vector3 PLAYER_CREATE_POSITION = Vector3(0.0f, 0.0f, -400.0f);

	const float AMBIENT_COLOR = 1.0f;

	const float SECOND = 60.0f;

	const float DEFAULT_BATTLE_BGM = 2.0f;

	const float MUTE_SPEED = 5.0f;
}

Game::Game()
{
}

Game::~Game()
{
	//全ての音をストップ
	g_soundManager->StopAllSound();

	DeleteGO(m_gameUI);

	DeleteGO(m_skyCube);
	DeleteGO(m_bossStage1);
	DeleteGO(m_player);
	DeleteGO(m_boss);
	DeleteGO(m_gameCamera);
	DeleteGO(m_gameFinishCamera);
	DeleteGO(m_battlePhase);
	DeleteGO(m_pause);
	DeleteGO(m_result);
}

bool Game::Start()
{
	//ライティングの初期化
	InitLighting();

	//ゲーム進行マネージャークラスの生成
	//ゲームシーンステートの設定
	GameManager::CreateInstance(GameManager::enGameSeenState_Game);
	//初期化処理
	GameManager::GetInstance()->Init();

	//エフェクト初期化クラスの生成。
	InitEffect* initEffect = new InitEffect();
	initEffect->InitEFK();
	delete initEffect;

	//ゲームオブジェクトの初期化
	InitGameObject();

	//フェードクラスのインスタンスを探す
	m_fade = FindGO<Fade>("fade");
	//画面を明るくする
	m_fade->StartFadeOut(2.0f);



	//被写界深度の無効化
	g_renderingEngine->DisableDof();
	//リムライトの有効化
	g_renderingEngine->UseLimLight();

	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();


	return true;
}

void Game::Update()
{	
	ManageState();
}

bool Game::Fadecomplete()
{
	//透明でないなら
	if (m_fade->GetCurrentAlpha() != 0.0f)
	{
		return false;
	}
	//透明なら
	return true;
}

void Game::CreateBoss()
{
	//ボスクラスの生成
	GameManager::GetInstance()->CreateBoss();
	m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();
	//モンスターが一定以上近づかないように毎フレーム調べる
	//数を取得できるようにする
}

void Game::CreatePlayerAndCamera()
{
	//プレイヤークラスの生成
	GameManager::GetInstance()->CreatePlayerClass();
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();
	//ゲームカメラの生成
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
}

void Game::CreateBattlePhase()
{
	//バトルフェーズ処理クラス生成
	//m_battlePhase = NewGO<BattlePhase>(0, "battlephase");
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetScale(800.0f);
	m_skyCube->SetPosition(m_skyPos);
	m_skyCube->SetLuminance(1.01f);
	m_skyCube->SetType(enSkyCubeType_DayToon_3);
	m_skyCube->Update();
}

void Game::GoResult()
{
	if (m_result == nullptr)
	{
		m_result = NewGO<ResultSeen>(0, "result");
	}
	//画面がリザルトの画像になった
	//円形ワイプが終わったら
	//todo円形ワイプの間音を小さくしていく
	/*if (m_result->GetRoundWipeEndFlag() == true)
	{
		DeleteGO(this);
	}*/

	//リザルトの処理が終わったら、ゲームを終わる処理をする
	if (m_result->GetResultEndFlag() == true)
	{
		//最後のステートに進む
		GameManager::GetInstance()->
			SetGameSeenState(GameManager::enGameSeenState_GameEnd);
	}


}

bool Game::IsWinnerDecision()
{
	return GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_GameOver ||
		GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_GameClear;
}

void Game::CalcMuteBGM()
{
	if (m_muteBGMFlag != true)
	{
		return;
	}

	m_bgmVolume = Math::Lerp(g_gameTime->GetFrameDeltaTime() * MUTE_SPEED, m_bgmVolume, 0.0f);

	if (m_bgmVolume < 0.1f)
	{
		m_muteBGMFlag = false;
		g_soundManager->StopSound(enSoundName_BattleBGM);
		return;
	}
	else
	{
		g_soundManager->GetSoundSource(enSoundName_BattleBGM)->SetVolume(m_bgmVolume);
	}
}

void Game::ManageState()
{
	//フラグが立っていたらBGMを小さくする
	CalcMuteBGM();

	switch (GameManager::GetInstance()->GetGameSeenState())
	{
		//ゲームスタート
	case GameManager::enGameSeenState_GameStart:
		OnProcessGameStartTransition();
		break;
		//ボス登場ムービー
	case GameManager::enGameSeenState_AppearanceBoss:
		OnProcessAppearanceBossTransition();
		break;
		//インゲーム
	case GameManager::enGameSeenState_Game:
		OnProcessGameTransition();
		break;
		//ポーズ画面
	case GameManager::enGameSeenState_Pause:
		OnProcessPauseTransition();
		break;
		//ゲームオーバー
	case GameManager::enGameSeenState_GameOver:
		OnProcessGameOverTransition();
		break;
		//ゲームクリア
	case GameManager::enGameSeenState_GameClear:
		OnProcessGameClearTransition();
		break;
		//ゲームエンド
	case GameManager::enGameSeenState_GameEnd:
		OnProcessGameEndTransition();
		break;
	default:
		break;
	}
}

void Game::OnProcessGameStartTransition()
{
	//画面が完全にフェードインしたら
	if (Fadecomplete() != true && m_fade->IsFade() == false)
	{
		//バトルスタートクラス削除
		DeleteGO(m_battleStart);
		//ボス登場クラス作成
		m_entryBoss = NewGO<EntryBoss>(0, "entryboss");
		m_entryBoss->SetPosition(BOSS_CREATE_POSITION);
		m_entryBoss->SetGame(this);
		m_entryBoss->SetSkyCube(m_skyCube);
		//プレイヤーとカメラの生成
		CreatePlayerAndCamera();

		//次のステップのステートに切り替える
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_AppearanceBoss);
	}
	return;
}

void Game::OnProcessAppearanceBossTransition()
{
	//ボスの出現アニメーションを再生する。終わったらボス生成
	//ボス登場ムービーが終わったら
	if (m_bossMovieEndFlag == true)
	{
		//次のステートに移る時
		//ムービー用のモデルを消す
		DeleteGO(m_entryBoss);
		//ボスのアクティブ化
		CreateBoss();

		//ステートを切り替える
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_Game);
		return;
	}

	//ボスの登場ムービークラス生成
	//if (m_entryBoss == nullptr)
	//{
	//	m_entryBoss = NewGO<EntryBoss>(0, "entryboss");
	//	m_entryBoss->SetPosition(BOSS_CREATE_POSITION);
	//	m_entryBoss->SetGame(this);
	//	m_entryBoss->SetSkyCube(m_skyCube);

	//	//プレイヤーとカメラの生成
	//	CreatePlayerAndCamera();
	//}
}

void Game::OnProcessGameTransition()
{
	switch (m_enGameStep)
	{
		//フェードアウト
	case Game::enGameStep_FadeOut:
		OnProcessGame_FadeOutTransition();
		break;
		//フェードイン
	case Game::enGameStep_FadeNone:
		OnProcessGame_FadeNoneTransition();
		break;
		//バトル中
	case Game::enGameStep_Battle:
		OnProcessGame_BattleTransition();
		break;
	default:
		break;
	}
}

void Game::OnProcessGameOverTransition()
{
	//バトル終了後の処理が終わったら
	if (GameManager::GetInstance()->GetGameFinishProcessEndFlag() == true)
	{
		//ゲーム画面からリザルト画面に遷移するまでの処理
		GoResult();
		return;
	}
	
}

void Game::OnProcessGameClearTransition()
{
	//バトル終了後の処理が終わったら
	if (GameManager::GetInstance()->GetGameFinishProcessEndFlag() == true)
	{
		GoResult();
		return;
	}
}

void Game::OnProcessPauseTransition()
{
	//ポーズ画面のゲーム終了フラグがtrueなら
	if (m_pause->GetGameExitFlag() == true)
	{
		//ローディング画面挟んでタイトルに戻る
		Title* title = NewGO<Title>(0, "title");
		//ゲームマネージャーの削除
		GameManager::DeleteInstance();
		DeleteGO(this);
		return;
	}

	//スタートボタンを押したらゲームに戻る
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		//ポーズ画面の削除
		DeleteGO(m_pause);
		//ゲームに戻る
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_Game);
		return;
	}
}

void Game::OnProcessGameEndTransition()
{
	//todo ローディング画面の生成
	Loading* loading = NewGO<Loading>(0, "loading");
	loading->SetLoadingRoot(Loading::enLoadingRoot_GameToTitle);

	//タイトルの生成
	//Title* title = NewGO<Title>(0, "title");
	DeleteGO(this);
}

void Game::OnProcessGame_FadeOutTransition()
{
	//フェード状態がなしならフェードアウトする
	if (m_fade->IsFade() == false)
	{
		//UI生成
		CreateGameUI();
		//バトルフェーズクラス生成の生成
		CreateBattlePhase();
		//このステートに入ってフェードアウトするとき
		m_fade->StartFadeOut(3.0f);
		//次のステップに進む
		m_enGameStep = enGameStep_FadeNone;
	}
}

void Game::OnProcessGame_FadeNoneTransition()
{
	if (m_fade->IsFade() == false)
	{
		//BGMの音量を上げる
		//BGMの再生
		g_soundManager->InitAndPlaySoundSource(enSoundName_BattleBGM, DEFAULT_BATTLE_BGM, false, true, true);
		//次のステップに進む
		m_enGameStep = enGameStep_Battle;
	}
}

void Game::OnProcessGame_BattleTransition()
{
	//勝敗が着いたら
	if (IsOutcome() == true)
	{
		return;
	}
	//スタートボタンを押したらポーズ画面
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		//ポーズ画面の生成
		m_pause = NewGO<Pause>(0, "pause");
		//ポーズ画面ステートに切り替え
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_Pause);
		return;
	}

	//ゲームマネージャーの毎フレームの更新処理
	GameManager::GetInstance()->Execute();
	//キャラクター情報マネージャーの毎フレームの更新処理
	CharactersInfoManager::GetInstance()->Execute();
}

bool Game::IsOutcome()
{
	//勝敗がついてないなら処理しない
	if (GameManager::GetInstance()->GetOutComeState() ==
		GameManager::enOutComeState_None)
	{
		return false;
	}

	//プレイヤーの勝ちなら
	if (GameManager::GetInstance()->GetOutComeState() ==
		GameManager::enOutComeState_PlayerWin)
	{
		ProcessWin();
	}
	//プレイヤーの負けなら
	else
	{
		ProcessLose();
	}
	return true;
}

void Game::ProcessWin()
{
	//ゲームUIの削除
	//DeleteGO(m_gameUI);
	//カメラの削除
	DeleteGO(m_gameCamera);

	//ゲーム終わったときのカメラ生成
	m_gameFinishCamera = NewGO<GameFinishCamera>(0, "gamefinishcamera");

	//BGMを消し始める
	m_muteBGMFlag = true;
	m_bgmVolume = g_soundManager->GetBGMVolume();

	//ステートを切り替える
	GameManager::GetInstance()->SetGameSeenState(
		GameManager::enGameSeenState_GameClear);

}

void Game::ProcessLose()
{
	//ゲームUIの削除
	//DeleteGO(m_gameUI);
	//BGMを消し始める
	m_muteBGMFlag = true;
	m_bgmVolume = g_soundManager->GetBGMVolume();
	//ステートを切り替える
	GameManager::GetInstance()->SetGameSeenState(
		GameManager::enGameSeenState_GameOver);
}

void Game::CreateGameUI()
{
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_gameUI->SetGame(this);
	m_gameUI->SetPlayer(CharactersInfoManager::GetInstance()->GetPlayerInstance());
	m_gameUI->SetBoss(CharactersInfoManager::GetInstance()->GetBossInstance());
}

void Game::InitGameObject()
{
	//スカイキューブの初期化
	InitSkyCube();
	//ボスステージの生成
	m_bossStage1 = NewGO<BossStage1>(0, "bossstage1");

	//ゲームシーンステートがゲームスタートなら
	if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_GameStart)
	{
		//バトルスタートクラス生成
		m_battleStart = NewGO<BattleStart>(0, "battlestart");
	}
	//ゲームシーンステートがゲームなら
	else if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_Game)
	{
		//ボスの生成
		CreateBoss();
		//プレイヤーとカメラクラスの生成
		CreatePlayerAndCamera();
	}

	
}

void Game::InitLighting()
{
	Vector3 directionLightDir = Vector3{ 1.0f,-1.0f,-1.0f };
	directionLightDir.Normalize();
	Vector3 directionLightColor = DIRECTION_RIGHT_COLOR;
	//ディレクションライト
	g_renderingEngine->SetDerectionLight(0, directionLightDir, directionLightColor);
	//環境光
	g_renderingEngine->SetAmbient(g_vec3One * AMBIENT_COLOR);
	//半球ライト
	g_renderingEngine->UseHemiLight();
}