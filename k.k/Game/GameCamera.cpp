#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"

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

	//�����_���王�_�܂ł̃x�N�g����ݒ�B80-160
	m_toCameraPos.Set(0.0f, 60.0f, 200.0f);
	//�J�������v���C���[�̌��ɂ���Ƃ��Ɏg��
	m_position = m_toCameraPos;

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}

void GameCamera::Update()
{
	//�����_�̌v�Z
	m_target = game->GetPosition();

	m_target.y += TARGETPOS_YUP;

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

	Vector3 finalCameraPos= m_toCameraPos + m_target;

	//���_�ƒ����_��ݒ�
	g_camera3D->SetTarget(m_target);
	g_camera3D->SetPosition(finalCameraPos);

	//�J�����̍X�V�B
	g_camera3D->Update();
}
