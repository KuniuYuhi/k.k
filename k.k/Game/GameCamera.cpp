#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"
#include "EntryBoss.h"
#include "Lich.h"

namespace {

	const float MAX_CAMERA_TOP = -0.1f;
	const float MAX_CAMERA_UNDER = 0.8f;

	const float TARGETPOS_YUP = 140.0f;
	const float TARGETPOS_GAMESTART_YUP = 38.0f;

	const float STARTCAMERA_YUP = 60.0f;

	const Quaternion START_ROT = { 0.0f,0.0f,0.0f,1.0f };
	const Quaternion CENTER_ROT = { 0.0f,0.0f,0.0f,1.0f };
	const Quaternion END_ROT = { -60.0f,90.0f,0.0f,1.0f };


	const Vector3 START_POS = { 0.0f,670.0f,800.0f };
	const Vector3 CENTER_POS = { 700.0f,470.0f,0.0f };
	const Vector3 END_POS = {160.0f,230.0f,-550.0f };

	const float PLAYER_CAMERA_X = 160.0f;
	const float PLAYER_CAMERA_Y = 0.0f;
	const float TARGETPOS_YUP_WIN = 40.0f;

	const float BOSS_CAMERA_X = 400.0f;
	const float BOSS_CAMERA_Y = 500.0f;
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

	m_toCameraPosForBoss.Set(0.0f, 300.0f, 600.0f);
	//注視点から視点までのベクトルを設定。300,400
	m_toCameraPos.Set(0.0f, 50.0f, 460.0f);
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
	//ChaseCamera();

	ManageState();

	
	
}

void GameCamera::ClearCameraForPlayer()
{
	//注視点の計算
	m_target = m_player->GetPosition();
	Vector3 forward = m_player->GetForward();
	forward.Normalize();
	//プレイヤーを見る
	GameClearCamera(
		m_target,
		forward,
		PLAYER_CAMERA_X,
		PLAYER_CAMERA_Y,
		TARGETPOS_YUP_WIN
	);
}

void GameCamera::ClearCameraForBoss()
{
	//注視点の計算
	m_target = m_lich->GetPosition();
	Vector3 forward = m_lich->GetForward();
	//リッチを見る
	GameClearCamera(
		m_target,
		forward,
		BOSS_CAMERA_X,
		BOSS_CAMERA_Y,
		TARGETPOS_YUP
	);
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
	m_toCameraPos.Set(newCameraPos);

	Vector3 finalCameraPos = newCameraPos + m_target;

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();
}

void GameCamera::ChaseBossCamera()
{
	if (m_time > 1.0f)
	{
		return;
	}

	//注視点の計算
	m_target = m_entryBoss->GetPosition();
	m_target.y = 0.0f;

	//線形補間
	m_pos1.Lerp(m_time, START_POS, CENTER_POS);
	m_pos2.Lerp(m_time, CENTER_POS, END_POS);
	m_toCameraPosForBoss.Lerp(m_time, m_pos1, m_pos2);

	m_time += g_gameTime->GetFrameDeltaTime() * 0.12f;

	Vector3 finalCameraPos = m_toCameraPosForBoss + m_target;

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

	Vector3 finalCameraPos;
	//カメラの位置の衝突解決する
	m_cameraCollisionSolver.Execute(
		finalCameraPos,
		m_target + m_toCameraPos,
		m_target
	);

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();
}

void GameCamera::GameStartCamera()
{
	//注視点の計算
	m_target = m_player->GetPosition();
	m_target.y += TARGETPOS_GAMESTART_YUP;
	//前方向の取得
	Vector3 CameraPosXZ = m_player->GetForward();
	CameraPosXZ.y = 0.0f;

	//XZ方向の
	CameraPosXZ *= 100.0f + m_count;
	//Y方向の
	Vector3 CameraPosY = Vector3::AxisY;
	CameraPosY *= STARTCAMERA_YUP +(m_count * 0.5f);

	//カメラの座標
	Vector3 newCameraPos = CameraPosXZ + CameraPosY;

	Vector3 finalCameraPos = newCameraPos + m_target;

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();

	m_count++;
}

void GameCamera::GameClearCamera(Vector3 targetPos, Vector3 forward, float X, float Y, float Yup, bool reversalFlag)
{
	//注視点の計算
	m_target = targetPos;
	//Y座標を上げる
	m_target.y += Yup;
	//前方向の取得
	Vector3 CameraPosXZ = forward;
	CameraPosXZ.y = 0.0f;
	//反転させる
	if (reversalFlag == true)
	{
		CameraPosXZ *= -1.0f;
	}
	//XZ方向の
	if (X > 0.0f)
	{
		CameraPosXZ *= X;
	}
	//Y方向の
	Vector3 CameraPosY = Vector3::AxisY;
	//0より大きければ
	if (Y > 0.0f)
	{
		CameraPosY *= Y;
	}
	

	//カメラの座標
	Vector3 newCameraPos = CameraPosXZ + CameraPosY;

	Vector3 finalCameraPos = newCameraPos + m_target;

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//カメラの更新。
	m_springCamera.Update();
}

void GameCamera::ManageState()
{
	//ゲームのステートによってカメラを切り替える
	switch (m_game->GetNowGameState())
	{
	case Game::enGameState_GameStart:
		//ゲームスタート
		OnProcessGameStartTransition();
		break;
	case Game::enGameState_AppearanceBoss:
		//ボスを見る
		OnProcessAppearanceBossTransition();
		break;
	case Game::enGameState_Game:
		//ゲーム中
		OnProcessGameTransition();
		break;
	case Game::enGameState_GameOver:
		//ゲームオーバー
		OnProcessGameOverTransition();
		break;
	case Game::enGameState_GameClear:
		//ゲームクリア
		OnProcessGameClearTransition();
		break;
	case Game::enGameState_Pause:

		break;
	default:
		break;
	}
}

void GameCamera::OnProcessGameStartTransition()
{
	GameStartCamera();
}

void GameCamera::OnProcessAppearanceBossTransition()
{
	if (m_entryBoss == nullptr)
	{
		m_entryBoss = FindGO<EntryBoss>("entryboss");
		return;
	}

	ChaseBossCamera();
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
}

void GameCamera::OnProcessGameOverTransition()
{
}

void GameCamera::OnProcessGameClearTransition()
{
	switch (m_game->GetClearCameraState())
	{
	case Game::enClearCameraState_Lich:
		ClearCameraForBoss();
		break;
	case Game::enClearCameraState_Player:
		ClearCameraForPlayer();
		break;

	default:
		break;
	}	
}

