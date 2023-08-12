#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"

namespace {
	const float TARGETPOS_YUP = 40.0f;
}

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	game = FindGO<Game>("game");

	m_player = FindGO<Player>("player");


	//注視点から視点までのベクトルを設定。300,400
	m_toCameraPos.Set(0.0f, 300.0f, 400.0f);
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
	ChaseCamera();
}

void GameCamera::ChaseCamera()
{
	//注視点の計算
	m_target = m_player->GetPosition();

	m_target.y += TARGETPOS_YUP;
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
	//Vector3 toPosDir = m_toCameraPos;
	//toPosDir.Normalize();
	//if (toPosDir.y < MAX_CAMERA_TOP) {
	//	//カメラが上向きすぎ。
	//	m_toCameraPos = toCameraPosOld;
	//}
	//else if (toPosDir.y > MAX_CAMERA_UNDER) {
	//	//カメラが下向きすぎ。
	//	m_toCameraPos = toCameraPosOld;
	//}

	Vector3 finalCameraPos = m_toCameraPos + m_target;
	
	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	//g_camera3D->SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();
}
