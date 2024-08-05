#include "stdafx.h"
#include "MobEnemyMovement.h"




MobEnemyMovement::MobEnemyMovement()
{
	Start();
}

MobEnemyMovement::~MobEnemyMovement()
{
}

void MobEnemyMovement::Start()
{
}

void MobEnemyMovement::UpdateComponent()
{
}

Vector3 MobEnemyMovement::CalcChaseCharacterVerocity(
	CommonEnemyStatus status, 
	Vector3 targetPositon,
	Vector3 m_currentPosition, Vector3 moveSpeed)
{
	Vector3 currentMoveSpeed = moveSpeed;
	currentMoveSpeed.x = 0.0f;
	currentMoveSpeed.z = 0.0f;

	
	Vector3 diff = targetPositon - m_currentPosition;
	diff.Normalize();
	diff.y = 0.0f;

	diff *= status.GetDefaultSpeed();

	currentMoveSpeed += diff;

	return currentMoveSpeed;
}
