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
	/// �o�g���J�n���̃J����
	/// </summary>
	void SetBattleStartCamera();

	/// <summary>
	/// �v���C���[��ǂ��J����
	/// </summary>
	/// <param name="Reversesflag">���_�𔽑Ε����ɂ��邩�̃t���O�Btrue�Ŕ��Ε����ɂ���</param>
	void ChasePlayerCamera(bool Reversesflag = false);

	/// <summary>
	/// �J�����̃Y�[������
	/// </summary>
	void ZoomCamera();

	/// <summary>
	/// �J�����̃Q�[�����̏���
	/// </summary>
	void OnProcessGameTransition();

	/// <summary>
	/// �J������h�炷
	/// </summary>
	void CameraShake();

	Vector3 GetUpdateShakePosition(
		float currentTimer);

	/// <summary>
	/// �l��͈͓��Ɏ��߂�
	/// </summary>
	/// <param name="value"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	float Clamp(float value, float min, float max);



	/// <summary>
	/// �X�v�����O�J�����̃��t���b�V��
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

	Vector3				m_toCameraPos = Vector3::Zero;		//�J�����ʒu���璍���_�Ɍ������x�N�g��
	Vector3				m_toCameraPosForBoss = Vector3::Zero;	//�J�����ʒu���璍���_(�{�X)�Ɍ������x�N�g��

	Vector3				m_pos1 = Vector3::Zero;
	Vector3				m_pos2 = Vector3::Zero;

	Vector3				m_position = Vector3::Zero;		//�J�������W
	Vector3				m_target = Vector3::Zero;		//�J���������_
	Quaternion			m_rotation = Quaternion::Identity;	//��]

	Vector3				m_finalCameraPos = g_vec3Zero;

	Vector3 m_shakeCameraPosition = g_vec3Zero;

	float m_shakeTimer = 0.0f;



	bool flag = false;

	bool SetBattleCameraFlag = false;



};

