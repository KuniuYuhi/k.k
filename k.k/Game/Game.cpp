#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "BossStage1.h"
#include "Lich.h"
#include "Result.h"


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

	fontTest.SetText(L"歯");
	fontTest.SetPosition(Vector3(50.0f, 0.0f, 0.0f));
	fontTest.SetColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	fontTest.SetOffset(Vector2(20.0f, -20.0f));

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

	spriteTest.Init("Assets/sprite/titleBack.DDS", 1920.0f, 1080.0f);
	spriteTest.SetPosition(m_position);
	//単純なリニアワイプ
	//spriteTest.SetSimpleWipe(true);
	// 方向を指定するリニアワイプ
	/*spriteTest.SetWipeWithDirection(true);
	spriteTest.SetDirection(5.0f, 5.0f);*/
	//円形ワイプ
	spriteTest.SetRoundWipe(true);
	spriteTest.SetRoundWipeStartPosition(1920.0f / 2, 1080.0f / 2);
	spriteTest.Update();


	m_bossStage1 = NewGO<BossStage1>(0, "bossstage1");

	m_player = NewGO<Player>(0, "player");

	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//m_lich = NewGO<Lich>(0, "lich");


	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();


	return true;
}

void Game::Update()
{
	//ボスがやられたら
	if (m_DeathBossFlag == true||m_playerAnnihilationFlag==true)
	{
		//リザルト画面に遷移するまでの処理
		GoResult();
		return;
	}
	

	Spotmove();

	if (g_pad[0]->IsPress(enButtonStart))
	{
		//spriteTest.SetSimpleWipe(true);
		spriteTest.SetWipeSize(wipSize);
		wipSize += 5.0f;
		//if (g_renderingEngine->HemiLightIsUse() == false)
		//{
		//	//g_renderingEngine->UnUseHemiLight();

		//	g_renderingEngine->UseHemiLight();
		//}
		//else
		//g_renderingEngine->UnUseHemiLight();
		
		//g_renderingEngine->UseHemiLight();
	}
}

void Game::GoResult()
{
	if (m_createResultFlag==false)
	{
		m_result = NewGO<ResultSeen>(0, "result");
		m_createResultFlag = true;
		
	}

	if (m_result->GetRoundWipeEndFlag() == true)
	{
		DeleteGO(this);
	}
}

void Game::SpriteTransform()
{
	//// 左スティック(キーボード：WASD)で平行移動。
	m_position.x += g_pad[0]->GetLStickXF();
	//m_position.y += g_pad[0]->GetLStickYF();

	// 右スティック(キーボード：上下左右)で回転。
	/*m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);*/

	// 上下左右キー(キーボード：2, 4, 6, 8)で拡大
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}

	spriteTest.SetPosition(m_position);
	//spriteTest.SetRotation(m_rotation);
	spriteTest.SetScale(m_scale);
	spriteTest.Update();
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
	//Tree.Draw(rc);
	
	//backGround.Draw(rc);
	
	//spriteTest.Draw(rc);
	//fontTest.Draw(rc);
}
