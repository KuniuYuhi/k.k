#include "stdafx.h"
#include "PlayerMovement.h"

#include "Brave.h"

#include "Status_Player.h"

PlayerMovement::PlayerMovement()
{
	Start();
}

PlayerMovement::~PlayerMovement()
{

}

void PlayerMovement::Start()
{
	m_brave = FindGO<Brave>("Brave");
}

void PlayerMovement::UpdateComponent()
{
	int a = 0;

}

Vector3 PlayerMovement::CalcSimpleMovementVerocity(
	float speed, Vector3 moveSpeed, Vector3 StickInput)
{
	Vector3 currentMoveSpeed = moveSpeed;
	currentMoveSpeed.x = 0.0f;
	currentMoveSpeed.z = 0.0f;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	//���K��
	cameraForward.Normalize();
	cameraRight.Normalize();

	//�J�����̕����Ƃ��ꂼ��̎��̓��͗ʂƃX�s�[�h�������Ĉړ��ʂ��v�Z
	cameraRight *= StickInput.x * speed;
	cameraForward *= StickInput.y * speed;

	//�O�����ƉE�����̈ړ��ʃx�N�g���𑫂�
	currentMoveSpeed += cameraRight + cameraForward;


	//�O������ݒ�
	if (fabsf(currentMoveSpeed.x) >= 0.001f || fabsf(currentMoveSpeed.z) >= 0.001f)
	{
		m_brave->SetForward(currentMoveSpeed);
	}


	return currentMoveSpeed;
}

Vector3 PlayerMovement::CalcForwardDirection(Vector3 forward, Vector3 moveSpeed)
{
	Vector3 currentMoveSpeed = moveSpeed;
	currentMoveSpeed.x = 0.0f;
	currentMoveSpeed.z = 0.0f;

	float y = currentMoveSpeed.y;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	//���K��
	cameraForward.Normalize();
	cameraRight.Normalize();

	//�����i�O�����j
	Vector3 direction = forward;
	direction.Normalize();

	//�J�����̕����Ƃ��ꂼ��̎��̓��͗ʂƃX�s�[�h�������Ĉړ��ʂ��v�Z
	cameraRight *= direction.x;
	cameraForward *= direction.z;

	//�O�����ƉE�����̈ړ��ʃx�N�g���𑫂�
	currentMoveSpeed += cameraRight + cameraForward;
	currentMoveSpeed.Normalize();

	//currentMoveSpeed.y = y;

	return currentMoveSpeed;
}

Vector3 PlayerMovement::CalcMoveDirection(Vector3 forward, Vector3 stick, Vector3 moveSpeed)
{
	Vector3 currentMoveSpeed = moveSpeed;
	currentMoveSpeed.x = 0.0f;
	currentMoveSpeed.z = 0.0f;

	float y = currentMoveSpeed.y;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	//���K��
	cameraForward.Normalize();
	cameraRight.Normalize();

	//�����i�O�����j
	Vector3 direction = forward;

	if (fabsf(stick.x) >= 0.001f ||
		fabsf(stick.y) >= 0.001f)
	{
		//�ړ��������X�e�B�b�N�̂��̂ɂ���
		direction = stick;
	}
	else
	{
		//�ړ��ʂ̌v�Z�̂��߂ɒl��ύX
		direction.y = direction.z;
	}

	direction.Normalize();

	//�J�����̕����Ƃ��ꂼ��̎��̓��͗ʂƃX�s�[�h�������Ĉړ��ʂ��v�Z
	cameraRight *= direction.x;
	cameraForward *= direction.y;

	//�O�����ƉE�����̈ړ��ʃx�N�g���𑫂�
	currentMoveSpeed += cameraRight + cameraForward;
	currentMoveSpeed.Normalize();

	//currentMoveSpeed.y = y;

	return currentMoveSpeed;
}
