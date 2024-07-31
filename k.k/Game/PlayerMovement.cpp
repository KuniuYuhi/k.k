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

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	//正規化
	forward.Normalize();
	right.Normalize();

	//
	right *= StickInput.x;
	forward *= StickInput.y;

	//前方向と右方向のベクトルを足す
	moveSpeed += right + forward;

	moveSpeed *= playerStatus.GetDefaultSpeed();
	moveSpeed.y = 0.0f;

	return moveSpeed;
}
