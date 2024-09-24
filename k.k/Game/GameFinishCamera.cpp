#include "stdafx.h"
#include "GameFinishCamera.h"

#include "GameSceneManager.h"

#include "Summoner.h"

#include "Result.h"

#include "Brave.h"


GameFinishCamera::GameFinishCamera()
{
}

GameFinishCamera::~GameFinishCamera()
{
	if (m_result != nullptr)
	{
		DeleteGO(m_result);
	}

}

bool GameFinishCamera::Start()
{
	//ボスのインスタンスの取得
	//m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();
	//プレイヤーのインスタンスの取得
	m_brave = FindGO<Brave>("Brave");

	
	Summoner* summoner = FindGO<Summoner>("Summoner");

	m_cameraCollisionSolver.Init(1.0f);
	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,			//カメラの移動速度の最大値。
		false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	//注視点の計算
	m_target = summoner->GetPosition();
	m_forward = summoner->GetForwardYZero();

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
	//ボスが消去されたら次はプレイヤーを見る。
	if (GameSceneManager::GetInstance()->IsBossDelete())
	{
		//ターゲットをプレイヤーに変更
		//プレイヤーの位置と前方向を設定
		m_target = m_brave->GetPosition();
		m_forward = m_brave->GetForward();
		m_forward.Normalize();
		m_forward.y = 0.0f;
		m_springCamera.Refresh();



		//次のステートに進む
		m_enFinishCameraState = enFinishCameraState_ChasePlayer;


		//プレイヤーのステートを勝利ステートに切り替え
		m_brave->ChangePlayerWinState();

		//リザルト画像クラスを生成
		m_result = NewGO<ResultSeen>(0, "result");

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
	
	//全ての処理が終わったのでリザルトシーンに移っても良いようにする
	//GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(true);

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

