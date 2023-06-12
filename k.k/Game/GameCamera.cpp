#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"

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

	//注視点から視点までのベクトルを設定。80-160
	m_toCameraPos.Set(0.0f, 60.0f, 200.0f);
	//カメラをプレイヤーの後ろにするときに使う
	m_position = m_toCameraPos;

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}

void GameCamera::Update()
{
	//注視点の計算
	m_target = game->GetPosition();

	m_target.y += TARGETPOS_YUP;

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

	Vector3 finalCameraPos= m_toCameraPos + m_target;

	//視点と注視点を設定
	g_camera3D->SetTarget(m_target);
	g_camera3D->SetPosition(finalCameraPos);

	//カメラの更新。
	g_camera3D->Update();
}
