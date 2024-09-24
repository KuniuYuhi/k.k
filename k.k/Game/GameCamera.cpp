#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"

#include "GameSceneManager.h"

#include "Brave.h"


//todo 画面を揺らす

namespace {

	const float MAX_CAMERA_TOP = -0.1f;
	const float MAX_CAMERA_UNDER = 0.8f;
	//150.0f
	const float TARGETPOS_YUP = 100.0f;
	
	const Vector3 DEFAULT_TOCAMERAPOS = { 0.0f, 200.0f, -400.0f };
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
	//m_game = FindGO<Game>("game");

	m_player = FindGO<Brave>("Brave");

	

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	
	//注視点から視点までのベクトルを設定。300,400
	m_toCameraPos.Set(DEFAULT_TOCAMERAPOS);
	//カメラをプレイヤーの後ろにするときに使う
	m_position = m_toCameraPos;

	m_cameraCollisionSolver.Init(1.0f);

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,			//カメラの移動速度の最大値。
		false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	


	//
	SetBattleCameraFlag = true;

	return true;
}

void GameCamera::Update()
{
	if (GameSceneManager::GetInstance()->IsGameOutcome())
	{
		return;
	}

	//カメラの処理
	OnProcessGameTransition();

	//カメラの位置からディレクションライトの方向を計算
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
	//カメラをリフレッシュ
	m_springCamera.Refresh();

	//注視点の計算
	m_target = m_player->GetPosition();
	m_target.y += TARGETPOS_YUP;
	//前方向の取得
	Vector3 CameraPosXZ = m_player->GetForward();
	CameraPosXZ.y = 0.0f;
	//反転
	CameraPosXZ *= -1.0f;
	//XZ方向
	CameraPosXZ *= 400.0f;
	//Y方向
	Vector3 CameraPosY = Vector3::AxisY;
	CameraPosY *= 100.0f;

	//新しいカメラの座標
	Vector3 newCameraPos = CameraPosXZ + CameraPosY;
	//最終的なカメラの座標
	Vector3 finalCameraPos = newCameraPos + m_target;

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();

	//
	SetBattleCameraFlag = true;
}

void GameCamera::ChasePlayerCamera(bool Reversesflag)
{
	//注視点の計算
	m_target = m_player->GetPosition();
	m_target.y = TARGETPOS_YUP;

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
}


void GameCamera::OnProcessGameTransition()
{
	//最初だけ
	if (SetBattleCameraFlag == false)
	{
		return;
	}
	
	//プレイヤーを追う
	ChasePlayerCamera();

	//カメラを揺らすなら
	if (g_camera3D->IsCameraShakeActive())
	{
		//カメラを揺らす
		CameraShake();
	}


	
	//ZoomCamera();

}

void GameCamera::CameraShake()
{
	m_shakeCameraPosition = GetUpdateShakePosition(m_shakeTimer);

	//タイマーの計算
	if (m_shakeTimer > g_camera3D->GetShakeInfo().shakeTimeLimit)
	{
		//タイムリミットに達したら
		//カメラの揺れフラグをリセット
		g_camera3D->DeactiveCameraShake();
		//位置を初期位置に戻す
		m_shakeCameraPosition = g_camera3D->GetShakeInfo().startPosition;
		//タイマーリセット
		m_shakeTimer = 0.0f;
	}
	else
	{
		m_shakeTimer += g_gameTime->GetFrameDeltaTime();
	}


	m_springCamera.SetPosition(m_shakeCameraPosition);
	//カメラの更新。
	m_springCamera.Update();
}

Vector3 GameCamera::GetUpdateShakePosition(float currentTimer)
{
	int max = g_camera3D->GetShakeInfo().shakeStrength * 2 + 1;

	//ランダムな値を取得
	int ramdomX = rand() % max - g_camera3D->GetShakeInfo().shakeStrength;
	int ramdomY = rand() % max - g_camera3D->GetShakeInfo().shakeStrength;

	Vector3 shakePos = m_springCamera.GetPosition();

	shakePos.x += ramdomX;
	shakePos.y += ramdomY;

	//揺れの強さの最大値
	float vibrato = g_camera3D->GetShakeInfo().shakeVibrato;
	//時間の割合を求める
	float ratio = 1.0f - currentTimer / g_camera3D->GetShakeInfo().shakeTimeLimit;
	//割合によって、タイムリミットに近づくと揺れの強さが小さくなっていく
	vibrato *= ratio;

	shakePos.x =
		Clamp(
			shakePos.x, 
			g_camera3D->GetShakeInfo().startPosition.x - vibrato,
			g_camera3D->GetShakeInfo().startPosition.x + vibrato
		);
	shakePos.y =
		Clamp(
			shakePos.y, 
			g_camera3D->GetShakeInfo().startPosition.y - vibrato,
			g_camera3D->GetShakeInfo().startPosition.y + vibrato
		);


	return shakePos;
}

float GameCamera::Clamp(float value, float min, float max)
{
	if (value < min) return min;

	if (value > max) return max;

	return value;
}
