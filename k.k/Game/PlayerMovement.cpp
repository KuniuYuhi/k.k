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
	Status_Player playerStatus, Vector3 StickInput)
{
	Vector3 moveSpeed = Vector3::Zero;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;
	//���K��
	forward.Normalize();
	right.Normalize();

	//
	right *= StickInput.x;
	forward *= StickInput.y;

	//�O�����ƉE�����̃x�N�g���𑫂�
	moveSpeed += right + forward;

	moveSpeed *= playerStatus.GetDefaultSpeed();
	moveSpeed.y = 0.0f;

	return moveSpeed;
}
