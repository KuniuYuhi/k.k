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
	//�{�X�̃C���X�^���X�̎擾
	m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();
	//�v���C���[�̃C���X�^���X�̎擾
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();


	m_cameraCollisionSolver.Init(1.0f);
	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
		1000.0f,			//�J�����̈ړ����x�̍ő�l�B
		false,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		1.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	//�����_�̌v�Z
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
		//�^�[�Q�b�g���v���C���[�ɕύX
		//�v���C���[�̈ʒu�ƑO������ݒ�
		m_target = m_player->GetPosition();
		m_forward = m_player->GetForward();
		m_springCamera.Refresh();
		//���̃X�e�[�g�ɐi��
		m_enFinishCameraState = enFinishCameraState_ChasePlayer;
		return;
	}

	ChaseCharacterCamera(-500.0f, 500.0f);
}

void GameFinishCamera::OnProcessChasePlayerTransition()
{

	ChaseCharacterCamera(-180.0f, 40.0f,50.0f);
	//���̃X�e�[�g�ɐi��
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
	//XZ�����̐ݒ�
	Vector3 CameraPosXZ = m_forward;
	CameraPosXZ.y = 0.0f;
	//���]������(���ʂ�����悤�ɂ���)
	CameraPosXZ *= -1.0f;
	//XZ�����ɐL�΂�
	CameraPosXZ *= xzDistance;
	//Y�����̐ݒ�
	Vector3 CameraPosY = Vector3::AxisY;
	CameraPosY *= yDistance;
	//�V�����J�����̎��_
	Vector3 newCameraPos = CameraPosXZ + CameraPosY;
	//�ŏI�I�ȃJ�����̎��_
	Vector3 finalCameraPos = newCameraPos + target;

	//���_�ƒ����_��ݒ�
	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(finalCameraPos);
	//�J�����̍X�V�B
	m_springCamera.Update();
}

