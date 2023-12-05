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
	/// �Q�[���N���A���Ƀv���C���[������J����
	/// </summary>
	void ClearCameraForPlayer();
	/// <summary>
	/// �Q�[���N���A����BOSS������J����
	/// </summary>
	void ClearCameraForBoss();

	/// <summary>
	/// �o�g���J�n���̃J����
	/// </summary>
	void SetBattleStartCamera();

	/// <summary>
	/// �v���C���[��ǂ��J����
	/// </summary>
	/// <param name="Reversesflag">���_�𔽑Ε����ɂ��邩�̃t���O�Btrue�Ŕ��Ε����ɂ���</param>
	void ChaseCamera(bool Reversesflag = false);

	/// <summary>
	/// �Q�[���N���A���̃J����
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

	/// <summary>
	/// �J�����̃Y�[������
	/// </summary>
	void ZoomCamera();

	void ManageState();

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

	Vector3				m_toCameraPos = Vector3::Zero;		//�J�����ʒu���璍���_�Ɍ������x�N�g��
	Vector3				m_toCameraPosForBoss = Vector3::Zero;	//�J�����ʒu���璍���_(�{�X)�Ɍ������x�N�g��

	Vector3				m_pos1 = Vector3::Zero;
	Vector3				m_pos2 = Vector3::Zero;

	Vector3				m_position = Vector3::Zero;		//�J�������W
	Vector3				m_target = Vector3::Zero;		//�J���������_
	Quaternion			m_rotation = Quaternion::Identity;	//��]

	Vector3				m_finalCameraPos = g_vec3Zero;

	bool flag = false;

	bool SetBattleCameraFlag = false;



};

