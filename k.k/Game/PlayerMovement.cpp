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
	Status_Player playerStatus, Vector3 moveSpeed, Vector3 StickInput)
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
	cameraRight *= StickInput.x * playerStatus.GetDefaultSpeed();
	cameraForward *= StickInput.y * playerStatus.GetDefaultSpeed();

	//�O�����ƉE�����̈ړ��ʃx�N�g���𑫂�
	currentMoveSpeed += cameraRight + cameraForward;


	return currentMoveSpeed;
}
