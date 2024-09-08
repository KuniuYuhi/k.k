#include "stdafx.h"
#include "BossEnemyMovement.h"

BossEnemyMovement::BossEnemyMovement()
{
}

BossEnemyMovement::~BossEnemyMovement()
{
}

void BossEnemyMovement::Start()
{
}

void BossEnemyMovement::UpdateComponent()
{
}

Vector3 BossEnemyMovement::CalcChaseCharacterVerocity(
	CommonEnemyStatus status, 
	Vector3 targetPositon, Vector3 m_currentPosition, Vector3 moveSpeed)
{
	Vector3 currentMoveSpeed = moveSpeed;
	currentMoveSpeed.x = 0.0f;
	currentMoveSpeed.z = 0.0f;

	//自身からターゲットに向かうベクトルを計算
	Vector3 diff = targetPositon - m_currentPosition;
	//正規化
	diff.Normalize();
	diff.y = 0.0f;

	//徐々に回転するようにするためにベクトルに制限をつける

	//前回の方向を前方向とする
	//m_previousDirection.Normalize();
	////
	//float d = Dot(m_previousDirection, diff);

	////あまり似ていないなら
	//if (d < 0)
	//{
	//	
	//	//ぎりぎりのベクトルに書き換える
	//	Vector3 cross = Cross(m_previousDirection, diff);
	//}


	//速さをかける
	diff *= status.GetDefaultSpeed();

	




	currentMoveSpeed += diff;

	return currentMoveSpeed;
}

Vector3 BossEnemyMovement::CalcChaseCharacterVerocity(
	float speed, Vector3 targetPositon, Vector3 m_currentPosition, Vector3 moveSpeed)
{
	Vector3 currentMoveSpeed = moveSpeed;
	currentMoveSpeed.x = 0.0f;
	currentMoveSpeed.z = 0.0f;


	Vector3 diff = targetPositon - m_currentPosition;
	diff.Normalize();
	diff.y = 0.0f;

	diff *= speed;

	currentMoveSpeed += diff;

	return currentMoveSpeed;
}

Vector3 BossEnemyMovement::AdjustDistance(Vector3 position)
{



	return Vector3();
}
