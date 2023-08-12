#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"

namespace {
	const float TARGETPOS_YUP = 40.0f;
}

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	game = FindGO<Game>("game");

	m_player = FindGO<Player>("player");


	//�����_���王�_�܂ł̃x�N�g����ݒ�B300,400
	m_toCameraPos.Set(0.0f, 300.0f, 400.0f);
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
	ChaseCamera();
}

void GameCamera::ChaseCamera()
{
	//�����_�̌v�Z
	m_target = m_player->GetPosition();

	m_target.y += TARGETPOS_YUP;
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
	//Vector3 toPosDir = m_toCameraPos;
	//toPosDir.Normalize();
	//if (toPosDir.y < MAX_CAMERA_TOP) {
	//	//�J����������������B
	//	m_toCameraPos = toCameraPosOld;
	//}
	//else if (toPosDir.y > MAX_CAMERA_UNDER) {
	//	//�J�����������������B
	//	m_toCameraPos = toCameraPosOld;
	//}

	Vector3 finalCameraPos = m_toCameraPos + m_target;
	
	//���_�ƒ����_��ݒ�
	m_springCamera.SetTarget(m_target);
	//g_camera3D->SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);

	//�J�����̍X�V�B
	m_springCamera.Update();
}
