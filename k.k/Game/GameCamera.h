#pragma once

#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Game;

class Brave;


class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();

	void CalcDirectionLight();

	/// <summary>
	/// バトル開始時のカメラ
	/// </summary>
	void SetBattleStartCamera();

	/// <summary>
	/// プレイヤーを追うカメラ
	/// </summary>
	/// <param name="Reversesflag">視点を反対方向にするかのフラグ。trueで反対方向にする</param>
	void ChasePlayerCamera(bool Reversesflag = false);

	/// <summary>
	/// カメラのズーム処理
	/// </summary>
	void ZoomCamera();

	/// <summary>
	/// カメラのゲーム中の処理
	/// </summary>
	void OnProcessGameTransition();

	/// <summary>
	/// カメラを揺らす
	/// </summary>
	void CameraShake();

	Vector3 GetUpdateShakePosition(
		float currentTimer);

	/// <summary>
	/// 値を範囲内に収める
	/// </summary>
	/// <param name="value"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	float Clamp(float value, float min, float max);



	/// <summary>
	/// スプリングカメラのリフレッシュ
	/// </summary>
	void CameraRefresh()
	{
		m_springCamera.SetDampingRate(1.0f);
		m_springCamera.Refresh();
	}

	



private:
	Game* m_game = nullptr;
	
	Brave* m_player = nullptr;


	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Vector3				m_toCameraPos = Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3				m_toCameraPosForBoss = Vector3::Zero;	//カメラ位置から注視点(ボス)に向かうベクトル

	Vector3				m_pos1 = Vector3::Zero;
	Vector3				m_pos2 = Vector3::Zero;

	Vector3				m_position = Vector3::Zero;		//カメラ座標
	Vector3				m_target = Vector3::Zero;		//カメラ注視点
	Quaternion			m_rotation = Quaternion::Identity;	//回転

	Vector3				m_finalCameraPos = g_vec3Zero;

	Vector3 m_shakeCameraPosition = g_vec3Zero;

	float m_shakeTimer = 0.0f;



	bool flag = false;

	bool SetBattleCameraFlag = false;



};

