#pragma once

#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Boss;
class Player;

class GameFinishCamera:public IGameObject
{
public:
	/// <summary>
	/// �Q�[�����I�������̃J��������
	/// </summary>
	enum EnFinishCameraState
	{
		enFinishCameraState_ChaseBoss,		//�{�X������
		enFinishCameraState_ChasePlayer,	//�v���C���[������
		enFinishCameraState_Finish,			//�I���
	};

	GameFinishCamera();
	~GameFinishCamera();

	bool Start();
	void Update();
private:
	/// <summary>
	/// ��ԊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �{�X������
	/// </summary>
	void OnProcessChaseBossTransition();
	/// <summary>
	/// �v���C���[������
	/// </summary>
	void OnProcessChasePlayerTransition();
	/// <summary>
	/// �I���
	/// </summary>
	void OnProcessFinishTransition();

	/// <summary>
	/// �L�����N�^�[��ǂ�������J����
	/// </summary>
	/// <param name="xzDistance">XZ�����ɐL�΂���������</param>
	/// <param name="yDistance">Y�����ɐL�΂���������</param>
	void ChaseCharacterCamera(float xzDistance, float yDistance,float yUp=0.0f);



private:
	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Boss* m_boss = nullptr;
	Player* m_player = nullptr;

	EnFinishCameraState m_enFinishCameraState = enFinishCameraState_ChaseBoss;

	Vector3				m_position = Vector3::Zero;		//�J�������W
	Vector3				m_target = Vector3::Zero;		//�J���������_
	Vector3 m_forward = g_vec3Zero;

};

