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

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//y方向には移動させない。
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	//正規化
	cameraForward.Normalize();
	cameraRight.Normalize();

	//カメラの方向とそれぞれの軸の入力量とスピードをかけて移動量を計算
	cameraRight *= StickInput.x * playerStatus.GetDefaultSpeed();
	cameraForward *= StickInput.y * playerStatus.GetDefaultSpeed();

	//前方向と右方向の移動量ベクトルを足す
	currentMoveSpeed += cameraRight + cameraForward;


	return currentMoveSpeed;
}
