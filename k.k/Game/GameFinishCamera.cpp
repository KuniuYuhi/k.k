#include "stdafx.h"
#include "GameFinishCamera.h"

#include "Boss.h"

#include "GameManager.h"
#include "CharactersInfoManager.h"

GameFinishCamera::GameFinishCamera()
{
}

GameFinishCamera::~GameFinishCamera()
{
}

bool GameFinishCamera::Start()
{
	//ボスのインスタンスの取得
	m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();
	//プレイヤーのインスタンスの取得
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();


	m_cameraCollisionSolver.Init(1.0f);
	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,			//カメラの移動速度の最大値。
		false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	//注視点の計算
	m_target = m_boss->GetPosition();
	m_forward = m_boss->GetForward();

	return true;
}

void GameFinishCamera::Update()
{
	ManageState();
}

void GameFinishCamera::ManageState()
{
	switch (m_enFinishCameraState)
	{
	case GameFinishCamera::enFinishCameraState_ChaseBoss:
		OnProcessChaseBossTransition();
		break;
	case GameFinishCamera::enFinishCameraState_ChasePlayer:
		OnProcessChasePlayerTransition();
		break;
	case GameFinishCamera::enFinishCameraState_Finish:
		OnProcessFinishTransition();
		break;
	default:
		break;
	}
}

void GameFinishCamera::OnProcessChaseBossTransition()
{
	//
	if (GameManager::GetInstance()->GetBossDeleteOkFlag() == true)
	{
		//ターゲットをプレイヤーに変更
		//プレイヤーの位置と前方向を設定
		m_target = m_player->GetPosition();
		m_forward = m_player->GetForward();
		m_springCamera.Refresh();
		//次のステートに進む
		m_enFinishCameraState = enFinishCameraState_ChasePlayer;
		return;
	}

	ChaseCharacterCamera(-500.0f, 500.0f);
}

void GameFinishCamera::OnProcessChasePlayerTransition()
{

	ChaseCharacterCamera(-180.0f, 40.0f,50.0f);
	//次のステートに進む
	m_enFinishCameraState = enFinishCameraState_Finish;
}

void GameFinishCamera::OnProcessFinishTransition()
{
	//
	GameManager::GetInstance()->SetGameFinishProcessEndFlag(true);
}

void GameFinishCamera::ChaseCharacterCamera(
	float xzDistance, float yDistance, float yUp)
{
	Vector3 target = m_target;
	target.y += yUp;
	//XZ方向の設定
	Vector3 CameraPosXZ = m_forward;
	CameraPosXZ.y = 0.0f;
	//反転させる(正面を見るようにする)
	CameraPosXZ *= -1.0f;
	//XZ方向に伸ばす
	CameraPosXZ *= xzDistance;
	//Y方向の設定
	Vector3 CameraPosY = Vector3::AxisY;
	CameraPosY *= yDistance;
	//新しいカメラの視点
	Vector3 newCameraPos = CameraPosXZ + CameraPosY;
	//最終的なカメラの視点
	Vector3 finalCameraPos = newCameraPos + target;

	//視点と注視点を設定
	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(finalCameraPos);
	//カメラの更新。
	m_springCamera.Update();
}

