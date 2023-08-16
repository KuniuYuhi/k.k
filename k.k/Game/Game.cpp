#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "BossStage1.h"
#include "Lich.h"
#include "Result.h"
#include "GameUI.h"
#include "Slime.h"
#include "TurtleShell.h"
#include "Cactus.h"
#include "Mushroom.h"
#include "Fade.h"


#include "SkyCube.h"

namespace {
	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(1.0f, 1.0f, 1.0f);

	const Vector3 SPOT_LIGHT_COLOR = Vector3(40.0f, 10.0f, 10.0f);
}

Game::Game()
{
}

Game::~Game()
{
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
	g_renderingEngine->SetAmbient(Vector3(0.5f, 0.5f, 0.5f));
	//ポイントライト
	/*g_renderingEngine->SetPointLight(
		Vector3(200.0f, 50.0f, 200.0f),
		Vector3(15.0f, 10.0f, 12.0f),
		Vector3(500.0f, 3.0f, 0.0f)
	);*/
	//スポットライト
	spPosition.y = 4.0f;
	spPosition.z = 100.0f;
	spDirection = Vector3(1.0f, -1.0f, 1.0f);
	spDirection.Normalize();

	/*g_renderingEngine->SetSpotLight(
		spPosition,
		SPOT_LIGHT_COLOR,
		Vector3(200.0f, 3.0f, 0.0f),
		spDirection,
		Vector3(90.0f, 40.0f, 0.0f)
	);*/

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

	//単純なリニアワイプ
	//spriteTest.SetSimpleWipe(true);
	// 方向を指定するリニアワイプ
	/*spriteTest.SetWipeWithDirection(true);
	spriteTest.SetDirection(5.0f, 5.0f);*/

	//フェードクラスのインスタンスを探す
	m_fade = FindGO<Fade>("fade");
	//スカイキューブの初期化
	InitSkyCube();

	m_bossStage1 = NewGO<BossStage1>(0, "bossstage1");
	m_player = NewGO<Player>(0, "player");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//model.Init("Assets/modelData/character/Wizard/Effect/FireBall.tkm");

	/*m_lich = NewGO<Lich>(0, "lich");
	m_lich->SetPosition({ 0.0f, 0.0f, -500.0f });*/
	/*Slime* slime = NewGO<Slime>(0, "slime");
	slime->SetPosition({ 0.0f, 0.0f, -300.0f });*/
	/*TurtleShell* turtleshell = NewGO<TurtleShell>(0, "turtleshell");
	turtleshell->SetPosition({ 0.0f, 0.0f, -500.0f });*/
	/*Cactus* cactus = NewGO<Cactus>(0, "cactus");
	cactus->SetPosition({ 0.0f, 0.0f, -700.0f });*/
	/*Mushroom* mushroom = NewGO<Mushroom>(0, "mushroom");
	mushroom->SetPosition({ 0.0f,0.0f,-600.0f });*/

	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_gameUI->GetGame(this);
	m_gameUI->GetPlayer(m_player);
	m_gameUI->GetLich(m_lich);

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
	//フェードアウト仕切らないと処理しない
	if (Fadecomplete() != true)
	{
		return;
	}









	/*Quaternion roty = Quaternion::Identity;
	roty.AddRotationDegY(g_gameTime->GetFrameDeltaTime() * 2000.0f);

	m_skyCube->SetRotation(roty);
	m_skyCube->Update();*/

	//ボスがやられたら
	if (m_DeathBossFlag == true||m_playerAnnihilationFlag==true)
	{
		//リザルト画面に遷移するまでの処理
		GoResult();
		return;
	}
	

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
	/*m_lich = NewGO<Lich>(0, "lich");
	m_lich->SetPosition({ 0.0f, 0.0f, -500.0f });*/
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

void Game::GoResult()
{
	if (m_createResultFlag==false)
	{
		m_result = NewGO<ResultSeen>(0, "result");
		m_createResultFlag = true;
		
	}

	//画面がリザルトの画像になった
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

void Game::Render(RenderContext& rc)
{
	//model.Draw(rc);
}
