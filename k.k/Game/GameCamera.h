#pragma once

#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Game;
class Player;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();

	void ChaseCamera();


private:
	Game* game = nullptr;
	Player* m_player = nullptr;

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Vector3				m_toCameraPos = Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3				m_position = Vector3::Zero;		//カメラ座標
	Vector3				m_target = Vector3::Zero;		//カメラ注視点
	Quaternion			m_rotation = Quaternion::Identity;	//回転
};

