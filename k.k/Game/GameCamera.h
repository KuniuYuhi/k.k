#pragma once

#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Game;
class Player;
class EntryBoss;
class Lich;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();

	void CalcDirectionLight();


	/// <summary>
	/// ゲームクリア時にプレイヤーを見るカメラ
	/// </summary>
	void ClearCameraForPlayer();
	/// <summary>
	/// ゲームクリア時にBOSSを見るカメラ
	/// </summary>
	void ClearCameraForBoss();

	/// <summary>
	/// バトル開始時のカメラ
	/// </summary>
	void SetBattleStartCamera();

	/// <summary>
	/// ボスを追うカメラ
	/// </summary>
	void ChaseBossCamera();

	/// <summary>
	/// プレイヤーを追うカメラ
	/// </summary>
	/// <param name="Reversesflag">視点を反対方向にするかのフラグ。trueで反対方向にする</param>
	void ChaseCamera(bool Reversesflag = false);

	/// <summary>
	/// ゲームスタート時のカメラ
	/// </summary>
	void GameStartCamera();

	/// <summary>
	/// ゲームクリア時のカメラ
	/// </summary>
	/// <param name="targetPos"></param>
	/// <param name="forward"></param>
	/// <param name="X"></param>
	/// <param name="Y"></param>
	/// <param name="Yupbool"></param>
	/// <param name="reversalFlag"></param>
	void GameClearCamera(
		Vector3 targetPos,
		Vector3 forward,
		float X,
		float Y,
		float Yup,
		bool reversalFlag = false
	);



	void ManageState();

	void OnProcessGameStartTransition();
	void OnProcessAppearanceBossTransition();
	void OnProcessGameTransition();
	void OnProcessGameOverTransition();
	void OnProcessGameClearTransition();


	void CameraRefresh()
	{
		m_springCamera.SetDampingRate(1.0f);
		m_springCamera.Refresh();
	}

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}



private:
	Game* m_game = nullptr;
	Player* m_player = nullptr;
	EntryBoss* m_entryBoss = nullptr;
	Lich* m_lich = nullptr;

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Vector3				m_toCameraPos = Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3				m_toCameraPosForBoss = Vector3::Zero;	//カメラ位置から注視点(ボス)に向かうベクトル

	Vector3				m_pos1 = Vector3::Zero;
	Vector3				m_pos2 = Vector3::Zero;

	Vector3				m_position = Vector3::Zero;		//カメラ座標
	Vector3				m_target = Vector3::Zero;		//カメラ注視点
	Quaternion			m_rotation = Quaternion::Identity;	//回転


	bool flag = false;

	bool SetBattleCameraFlag = false;

	float m_time = 0.0f;

	float m_count = 1.0f;

};

