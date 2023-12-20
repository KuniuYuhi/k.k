#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"
#include "Lich.h"
#include "GameManager.h"
#include "CharactersInfoManager.h"

namespace {

	const float MAX_CAMERA_TOP = -0.1f;
	const float MAX_CAMERA_UNDER = 0.8f;
	//150.0f
	const float TARGETPOS_YUP = 100.0f;
	
	const Vector3 DEFAULT_TOCAMERAPOS = { 0.0f, 250.0f, -600.0f };
	const Vector3 MAX_ZOOM_TOCAMERAPOS = { 0.0f,0.0f,-50.0f };

}

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");

	m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();


	m_toCameraPosForBoss.Set(0.0f, 200.0f, 200.0f);
	//注視点から視点までのベクトルを設定。300,400
	m_toCameraPos.Set(DEFAULT_TOCAMERAPOS);
	//カメラをプレイヤーの後ろにするときに使う
	m_position = m_toCameraPos;

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	m_cameraCollisionSolver.Init(1.0f);

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,			//カメラの移動速度の最大値。
		false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	

	return true;
}

void GameCamera::Update()
{
	if (m_game != nullptr)
	{
		ManageState();
	}
	else
	{
		OnProcessGameTransition();
	}
	
	CalcDirectionLight();
}

void GameCamera::CalcDirectionLight()
{
	Vector3 forward = g_camera3D->GetForward();
	forward.y = -0.3f;
	forward.x = 1.0f;
	forward.Normalize();

	//xy軸だけ動かす
	g_renderingEngine->SetDirLightDirection(forward);
}

void GameCamera::SetBattleStartCamera()
{
	//注視点の計算
	m_target = m_player->GetPosition();
	m_target.y += TARGETPOS_YUP;
	//前方向の取得
	Vector3 CameraPosXZ = m_player->GetForward();
	CameraPosXZ.y = 0.0f;
	//反転
	CameraPosXZ *= -1.0f;
	//XZ方向の
	CameraPosXZ *= 400.0f;
	//Y方向の
	Vector3 CameraPosY = Vector3::AxisY;
	CameraPosY *= 100.0f;

	//新しいカメラの座標
	Vector3 newCameraPos = CameraPosXZ + CameraPosY;

	//カメラの座標を設定
	//m_toCameraPos.Set(newCameraPos);

	Vector3 finalCameraPos = newCameraPos + m_target;

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();
}

void GameCamera::ChaseCamera(bool Reversesflag)
{
	//注視点の計算
	m_target = m_player->GetPosition();
	m_target.y = TARGETPOS_YUP;
	//m_target.x -= 35.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);

	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);

	//カメラの回転の上限をチェックする。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < MAX_CAMERA_TOP) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > MAX_CAMERA_UNDER) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	//Vector3 finalCameraPos;
	//カメラの位置の衝突解決する
	m_cameraCollisionSolver.Execute(
		m_finalCameraPos,
		m_target + m_toCameraPos,
		m_target
	);

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(m_finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();
}

void GameCamera::ZoomCamera()
{
	//LB押していないなら処理しない
	if (g_pad[0]->IsPress(enButtonLB1) != true)
	{
		return;
	}
	//ズームイン
	if (g_pad[0]->IsPress(enButtonUp))
	{
		m_toCameraPos.Lerp(g_gameTime->GetFrameDeltaTime()*2.0f, m_toCameraPos, MAX_ZOOM_TOCAMERAPOS);
		return;
	}
	//ズームアウト
	if (g_pad[0]->IsPress(enButtonDown))
	{
		m_toCameraPos.Lerp(g_gameTime->GetFrameDeltaTime()*2.0f, m_toCameraPos, DEFAULT_TOCAMERAPOS);
		return;
	}
	//todo デフォルトに戻す
	/*if (g_pad[0]->IsPress(EnButton::))
	{

	}*/

}

void GameCamera::ManageState()
{
	//ゲームのステートによってカメラを切り替える
	switch (GameManager::GetInstance()->GetGameSeenState())
	{
	case GameManager::enGameSeenState_Game:
		//ゲーム中
		OnProcessGameTransition();
		break;
	default:
		break;
	}
}

void GameCamera::OnProcessGameTransition()
{
	//最初だけ
	if (SetBattleCameraFlag == false)
	{
		//リフレッシュ
		m_springCamera.Refresh();
		SetBattleStartCamera();
		SetBattleCameraFlag = true;
	}
	
	//プレイヤーを追う
	ChaseCamera();

	ZoomCamera();

}
