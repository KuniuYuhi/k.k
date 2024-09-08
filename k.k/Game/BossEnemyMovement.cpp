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

	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z
	Vector3 diff = targetPositon - m_currentPosition;
	//���K��
	diff.Normalize();
	diff.y = 0.0f;

	//���X�ɉ�]����悤�ɂ��邽�߂Ƀx�N�g���ɐ���������

	//�O��̕�����O�����Ƃ���
	//m_previousDirection.Normalize();
	////
	//float d = Dot(m_previousDirection, diff);

	////���܂莗�Ă��Ȃ��Ȃ�
	//if (d < 0)
	//{
	//	
	//	//���肬��̃x�N�g���ɏ���������
	//	Vector3 cross = Cross(m_previousDirection, diff);
	//}


	//������������
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
