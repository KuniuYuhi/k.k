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
	cameraRight *= StickInput.x * speed;
	cameraForward *= StickInput.y * speed;

	//前方向と右方向の移動量ベクトルを足す
	currentMoveSpeed += cameraRight + cameraForward;


	//前方向を設定
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

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//y方向には移動させない。
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	//正規化
	cameraForward.Normalize();
	cameraRight.Normalize();

	//方向（前方向）
	Vector3 direction = forward;
	direction.Normalize();

	//カメラの方向とそれぞれの軸の入力量とスピードをかけて移動量を計算
	cameraRight *= direction.x;
	cameraForward *= direction.z;

	//前方向と右方向の移動量ベクトルを足す
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

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//y方向には移動させない。
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	//正規化
	cameraForward.Normalize();
	cameraRight.Normalize();

	//方向（前方向）
	Vector3 direction = forward;

	if (fabsf(stick.x) >= 0.001f ||
		fabsf(stick.y) >= 0.001f)
	{
		//移動方向をスティックのものにする
		direction = stick;
	}
	else
	{
		//移動量の計算のために値を変更
		direction.y = direction.z;
	}

	direction.Normalize();

	//カメラの方向とそれぞれの軸の入力量とスピードをかけて移動量を計算
	cameraRight *= direction.x;
	cameraForward *= direction.y;

	//前方向と右方向の移動量ベクトルを足す
	currentMoveSpeed += cameraRight + cameraForward;
	currentMoveSpeed.Normalize();

	//currentMoveSpeed.y = y;

	return currentMoveSpeed;
}
