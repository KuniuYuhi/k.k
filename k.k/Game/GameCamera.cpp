#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"
#include "Lich.h"
#include "GameManager.h"
#include "CharactersInfoManager.h"

namespace {

	const float MAX_CAMERA_TOP = -0.1f;
	const float MAX_CAMERA_UNDER = 0.8f;
	//150.0f
	const float TARGETPOS_YUP = 100.0f;
	
	const Vector3 DEFAULT_TOCAMERAPOS = { 0.0f, 250.0f, -600.0f };
	const Vector3 MAX_ZOOM_TOCAMERAPOS = { 0.0f,0.0f,-50.0f };

}

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");

	m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();


	m_toCameraPosForBoss.Set(0.0f, 200.0f, 200.0f);
	//�����_���王�_�܂ł̃x�N�g����ݒ�B300,400
	m_toCameraPos.Set(DEFAULT_TOCAMERAPOS);
	//�J�������v���C���[�̌��ɂ���Ƃ��Ɏg��
	m_position = m_toCameraPos;

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	m_cameraCollisionSolver.Init(1.0f);

	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
		1000.0f,			//�J�����̈ړ����x�̍ő�l�B
		false,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		1.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	

	return true;
}

void GameCamera::Update()
{
	if (m_game != nullptr)
	{
		ManageState();
	}
	else
	{
		OnProcessGameTransition();
	}
	
	CalcDirectionLight();
}

void GameCamera::CalcDirectionLight()
{
	Vector3 forward = g_camera3D->GetForward();
	forward.y = -0.3f;
	forward.x = 1.0f;
	forward.Normalize();

	//xy������������
	g_renderingEngine->SetDirLightDirection(forward);
}

void GameCamera::SetBattleStartCamera()
{
	//�����_�̌v�Z
	m_target = m_player->GetPosition();
	m_target.y += TARGETPOS_YUP;
	//�O�����̎擾
	Vector3 CameraPosXZ = m_player->GetForward();
	CameraPosXZ.y = 0.0f;
	//���]
	CameraPosXZ *= -1.0f;
	//XZ������
	CameraPosXZ *= 400.0f;
	//Y������
	Vector3 CameraPosY = Vector3::AxisY;
	CameraPosY *= 100.0f;

	//�V�����J�����̍��W
	Vector3 newCameraPos = CameraPosXZ + CameraPosY;

	//�J�����̍��W��ݒ�
	//m_toCameraPos.Set(newCameraPos);

	Vector3 finalCameraPos = newCameraPos + m_target;

	//���_�ƒ����_��ݒ�
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//�J�����̍X�V�B
	m_springCamera.Update();
}

void GameCamera::ChaseCamera(bool Reversesflag)
{
	//�����_�̌v�Z
	m_target = m_player->GetPosition();
	m_target.y = TARGETPOS_YUP;
	//m_target.x -= 35.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	
	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);

	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);

	//�J�����̉�]�̏�����`�F�b�N����B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < MAX_CAMERA_TOP) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > MAX_CAMERA_UNDER) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}

	//Vector3 finalCameraPos;
	//�J�����̈ʒu�̏Փˉ�������
	m_cameraCollisionSolver.Execute(
		m_finalCameraPos,
		m_target + m_toCameraPos,
		m_target
	);

	//���_�ƒ����_��ݒ�
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(m_finalCameraPos);

	//�J�����̍X�V�B
	m_springCamera.Update();
}

void GameCamera::ZoomCamera()
{
	//LB�����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (g_pad[0]->IsPress(enButtonLB1) != true)
	{
		return;
	}
	//�Y�[���C��
	if (g_pad[0]->IsPress(enButtonUp))
	{
		m_toCameraPos.Lerp(g_gameTime->GetFrameDeltaTime()*2.0f, m_toCameraPos, MAX_ZOOM_TOCAMERAPOS);
		return;
	}
	//�Y�[���A�E�g
	if (g_pad[0]->IsPress(enButtonDown))
	{
		m_toCameraPos.Lerp(g_gameTime->GetFrameDeltaTime()*2.0f, m_toCameraPos, DEFAULT_TOCAMERAPOS);
		return;
	}
	//todo �f�t�H���g�ɖ߂�
	/*if (g_pad[0]->IsPress(EnButton::))
	{

	}*/

}

void GameCamera::ManageState()
{
	//�Q�[���̃X�e�[�g�ɂ���ăJ������؂�ւ���
	switch (GameManager::GetInstance()->GetGameSeenState())
	{
	case GameManager::enGameSeenState_Game:
		//�Q�[����
		OnProcessGameTransition();
		break;
	default:
		break;
	}
}

void GameCamera::OnProcessGameTransition()
{
	//�ŏ�����
	if (SetBattleCameraFlag == false)
	{
		//���t���b�V��
		m_springCamera.Refresh();
		SetBattleStartCamera();
		SetBattleCameraFlag = true;
	}
	
	//�v���C���[��ǂ�
	ChaseCamera();

	ZoomCamera();

}
