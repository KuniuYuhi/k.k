#pragma once

#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Boss;
class Player;

class GameFinishCamera:public IGameObject
{
public:
	/// <summary>
	/// ゲームが終わった後のカメラ処理
	/// </summary>
	enum EnFinishCameraState
	{
		enFinishCameraState_ChaseBoss,		//ボスを見る
		enFinishCameraState_ChasePlayer,	//プレイヤーを見る
		enFinishCameraState_Finish,			//終わり
	};

	GameFinishCamera();
	~GameFinishCamera();

	bool Start();
	void Update();
private:
	/// <summary>
	/// 状態管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// ボスを見る
	/// </summary>
	void OnProcessChaseBossTransition();
	/// <summary>
	/// プレイヤーを見る
	/// </summary>
	void OnProcessChasePlayerTransition();
	/// <summary>
	/// 終わり
	/// </summary>
	void OnProcessFinishTransition();

	/// <summary>
	/// キャラクターを追いかけるカメラ
	/// </summary>
	/// <param name="xzDistance">XZ方向に伸ばしたい距離</param>
	/// <param name="yDistance">Y方向に伸ばしたい距離</param>
	void ChaseCharacterCamera(float xzDistance, float yDistance,float yUp=0.0f);



private:
	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Boss* m_boss = nullptr;
	Player* m_player = nullptr;

	EnFinishCameraState m_enFinishCameraState = enFinishCameraState_ChaseBoss;

	Vector3				m_position = Vector3::Zero;		//カメラ座標
	Vector3				m_target = Vector3::Zero;		//カメラ注視点
	Vector3 m_forward = g_vec3Zero;

};

