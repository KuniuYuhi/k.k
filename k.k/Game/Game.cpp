#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "BossStage1.h"
#include "Lich.h"
#include "Result.h"
#include "GameUI.h"
#include "Fade.h"
#include "EntryBoss.h"
#include "BattleStart.h"

#include "Slime.h"

#include "SkyCube.h"
#include "InitEffect.h"

namespace {
	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.6f, 0.6f, 0.6f);

	const Vector3 SPOT_LIGHT_COLOR = Vector3(40.0f, 10.0f, 10.0f);

	const Vector3 BOSS_CREATE_POSITION = Vector3(0.0f, 0.0f, 900.0f);

	const float AMBIENT_COLOR = 0.8f;

	const float SECOND = 60.0f;
}

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_skyCube);
	DeleteGO(m_bossStage1);
	
	DeleteGO(m_player);
	DeleteGO(m_lich);

	DeleteGO(m_gameCamera);

	DeleteGO(m_gameUI);
}

bool Game::Start()
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

	//レベル2D
	//level2DSp.Init(
	//	"Assets/Level2D/testLevel2D.casl",
	//	[&](Level2DObjectData& objData) {
	//		if (objData.EqualObjectName("pointer_black") == true)
	//		{
	//			spriteTest.Init("Assets/sprite/pointer_black.DDS", 220.0f, 220.0f);
	//			spriteTest.SetPosition(objData.position);
	//			//spriteTest.SetGrayScale(true);
	//			spriteTest.Update();

	//			return true;
	//		}

	//		return false;
	//	});


	//エフェクト初期化クラスの生成。
	InitEffect* initEffect = new InitEffect();
	initEffect->InitEFK();
	delete initEffect;

	//フェードクラスのインスタンスを探す
	m_fade = FindGO<Fade>("fade");
	//スカイキューブの初期化
	InitSkyCube();

	m_bossStage1 = NewGO<BossStage1>(0, "bossstage1");
	m_player = NewGO<Player>(0, "player");
	m_player->SetPosition({ 0.0f,0.0f,-500.0f });
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//ボスの生成。非アクティブにする
	CreateBoss();

	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//画面を明るくする
	m_fade->StartFadeOut(2.0f);
	//ゲームステートをスタートにする
	m_enGameState = enGameState_GameStart;

	return true;
}

void Game::Update()
{	
	ManageState();

	Spotmove();
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
	m_lich = NewGO<Lich>(0, "lich");
	m_lich->SetPosition(BOSS_CREATE_POSITION);
	m_lich->Deactivate();
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetScale(800.0f);
	m_skyCube->SetPosition(m_skyPos);
	m_skyCube->SetLuminance(0.9f);
	m_skyCube->SetType(enSkyCubeType_DayToon_3);

	m_skyCube->Update();
}

void Game::GoResult(EnOutCome outcome)
{
	if (m_result == nullptr)
	{
		m_result = NewGO<ResultSeen>(0, "result");
		switch (outcome)
		{
		case Game::enOutCome_Win:
			m_result->SetOutcome(ResultSeen::enOutcome_Win);
			break;
		case Game::enOutCome_Lose:
			m_result->SetOutcome(ResultSeen::enOutcome_Lose);
			break;
		default:
			break;
		}
	}
	//画面がリザルトの画像になった
	//円形ワイプが終わったら
	if (m_result->GetRoundWipeEndFlag() == true)
	{
		DeleteGO(this);
	}
}

void Game::Spotmove()
{
	/*spDirection.x -= g_pad[0]->GetLStickXF()*0.5f;
	spDirection.Normalize();
	g_renderingEngine->SetDirLightDirection(spDirection);*/


	//左のアナログスティックで動かす
	spPosition.x -= g_pad[0]->GetLStickXF();
	if (g_pad[0]->IsPress(enButtonB))
	{
		//Bボタンが一緒に押されていたらY軸方向に動かす
		spPosition.y += g_pad[0]->GetLStickYF();
	}
	else
	{
		//Z方向に動かす
		spPosition.z -= g_pad[0]->GetLStickYF();
	}

	// step-4 コントローラー右スティックでスポットライトを回転させる
	   //Y軸回りのクォータニオンを計算する
	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
	//計算したクォータニオンでライトの方向を回す
	qRotY.Apply(spDirection);

	//X軸回りのクォータニオンを計算する
	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, spDirection);
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);
	//計算したクォータニオンでライトの方向を回す
	qRotX.Apply(spDirection);

	spDirection.Normalize();
	/*g_renderingEngine->SetSpotLightPosition(spPosition);
	g_renderingEngine->SetSpotLightDirection(spDirection);*/
	
}

bool Game::CalcTimeLimit()
{
	//0秒以下なら
	if (m_second <= 0) {
		//1分減らす
		m_minute--;
		//もし0分なら、秒も0にする
		if (m_minute < 0) {
			m_second = 0.0f;
			m_minute = 0.0f;
			//制限時間に達した
			// ゲームオーバー
			return true;
		}
		//60秒に戻す
		else
		{
			m_second = SECOND;
		}
	}
	else
	{
		//秒を減らす
		m_second -= g_gameTime->GetFrameDeltaTime();
	}
	return false;
}

bool Game::IsBossMovieSkipTime()
{
	if (g_pad[0]->IsPress(enButtonA))
	{
		//3秒たったらスキップ
		if (m_bossMovieSkipTime < m_bossMovieSkipTimer)
		{
			return true;
		}
		else
		{
			m_bossMovieSkipTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
	else
	{
		m_bossMovieSkipTimer = 0.0f;
	}

	return false;
}

void Game::ManageState()
{
	switch (m_enGameState)
	{
	case enGameState_Fade:
		break;
	case enGameState_GameStart:
		OnProcessGameStartTransition();
		break;
	case enGameState_AppearanceBoss:
		OnProcessAppearanceBossTransition();
		break;
	case enGameState_Game:
		OnProcessGameTransition();
		break;
	case enGameState_Pause:
		break;
	case enGameState_GameOver:
		OnProcessGameOverTransition();
		break;
	case enGameState_GameOver_TimeUp:
		OnProcessGameOverTransition();
		break;
	case enGameState_GameClear:
		OnProcessGameClearTransition();
		break;
	default:
		break;
	}



}

void Game::OnProcessGameStartTransition()
{
	//一度だけバトルスタートクラス生成
	if (m_battleStart == nullptr)
	{
		m_battleStart = NewGO<BattleStart>(0, "battlestart");
	}

	//フェードアウト仕切らないと処理しない
	if (Fadecomplete() != true)
	{
		//画面が完全にフェードインしたら
		if (m_fade->IsFade()==false && m_enFadeState == enFadeState_StartToBoss)
		{

			DeleteGO(m_battleStart);
			//次のステップ
			//ステートを切り替える
			SetNextGameState(enGameState_AppearanceBoss);
			//カメラをリセットする
			m_gameCamera->CameraRefresh();

		}

		return;
	}
	//プレイヤーを見ている
	m_cameraZoomOutTimer += g_gameTime->GetFrameDeltaTime();
	//ある程度ズームしたら
	if (m_cameraZoomOutTime < m_cameraZoomOutTimer)
	{
		//フェードインを始める
		m_enFadeState = enFadeState_StartToBoss;
		m_fade->StartFadeIn(3.0f);
	}
}

void Game::OnProcessAppearanceBossTransition()
{
	//ボスの出現アニメーションを再生する。終わったらボス生成

	//フェードに入っている状態なら
	//フェードインしきったらステートを切り替える
	if (m_fade->IsFade() == false && m_enFadeState!=enFadeState_None)
	{
		switch (m_enFadeState)
		{
		case Game::enFadeState_StartToBoss:
			//このステートに入ってフェードアウトするとき
			m_fade->StartFadeOut(3.0f);
			//フェードステートをなしにする
			m_enFadeState = enFadeState_None;
			break;
		case Game::enFadeState_BossToPlayer:
			//次のステートに移る時
			//ムービー用のモデルを消す
			DeleteGO(m_entryBoss);
			//ボスのアクティブ化
			m_lich->Activate(); 
			//ステートを切り替える
			SetNextGameState(enGameState_Game);
			break;
		default:
			break;
		}
		return;
	}

	//スキップ処理
	if (IsBossMovieSkipTime() == true)
	{
		//フェードインを始める
		m_enFadeState = enFadeState_BossToPlayer;
		m_fade->StartFadeIn(3.0f);
		return;
	}

	//ボスの登場ムービークラス生成
	if (m_entryBoss == nullptr)
	{
		m_entryBoss = NewGO<EntryBoss>(0, "entryboss");
		m_entryBoss->SetPosition(BOSS_CREATE_POSITION);
		m_entryBoss->SetGame(this);
		m_entryBoss->SetSkyCube(m_skyCube);
	}
	//ボスの登場ムービーが終わったら
	if (m_bossMovieEndFlag == true)
	{
		//フェードインを始める
		m_enFadeState = enFadeState_BossToPlayer;
		m_fade->StartFadeIn(3.0f);
	}
}

void Game::OnProcessGameTransition()
{
	//制限時間の処理
	if (CalcTimeLimit() == true)
	{
		//タイムアップ
		DeleteGO(m_gameUI);
		//ステートを切り替える
		SetNextGameState(enGameState_GameOver_TimeUp);
	}


	//ゲームクリア
	//ボスがやられたら
	if (m_DeathBossFlag == true)
	{
		DeleteGO(m_gameUI);
		//ステートを切り替える
		SetNextGameState(enGameState_GameClear);
		//
		m_gameCamera->SetLich(m_lich);
		//カメラをリセットする
		m_gameCamera->CameraRefresh();
		return;
	}
	//ゲームオーバー
	//キャラクターが全滅したら
	if (m_playerAnnihilationFlag == true)
	{
		DeleteGO(m_gameUI);
		//ステートを切り替える
		SetNextGameState(enGameState_GameOver);
	}



	//画面を明るくする
	if (m_fade->IsFade() == false && m_enFadeState == enFadeState_BossToPlayer)
	{
		//UI生成
		m_gameUI = NewGO<GameUI>(0, "gameUI");
		m_gameUI->GetGame(this);
		m_gameUI->GetPlayer(m_player);
		m_gameUI->GetLich(m_lich);
		//このステートに入ってフェードアウトするとき
		m_fade->StartFadeOut(3.0f);
		//フェードステートをなしにする
		m_enFadeState = enFadeState_None;
	}
}

void Game::OnProcessGameOverTransition()
{
	//ゲーム画面からリザルト画面に遷移するまでの処理
	GoResult(enOutCome_Lose);
}

void Game::OnProcessGameClearTransition()
{
	if (m_displayResultFlag == true)
	{
		GoResult(enOutCome_Win);
		return;
	}
	//リザルト画面がない間
	//リッチがいる間はカメラに移す
	m_lich = FindGO<Lich>("lich");
	if (m_lich == nullptr)
	{
		//フレームレートを落とす
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);
		//ボスがいなくなったらカメラの対象を変える
		SetClearCameraState(Game::enClearCameraState_Player);
		//リザルト画面表示
		m_displayResultFlag = true;
	}

}
