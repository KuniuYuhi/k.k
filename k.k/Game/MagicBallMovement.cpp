#include "stdafx.h"
#include "MagicBallMovement.h"
#include "IMagicBall.h"


namespace {
	float MUL_STRAGHT_SPEED = 10.0f;
	float MUL_CHASE_SPEED = 4.0f;
}

MagicBallMovement::MagicBallMovement()
{
}

MagicBallMovement::~MagicBallMovement()
{
}

void MagicBallMovement::Start()
{
}

void MagicBallMovement::UpdateComponent()
{
}

void MagicBallMovement::MoveStraight(float yUp, bool setYUp)
{
	//�O�������擾
	Vector3 moveSpeed = m_magicBall->GetForward();
	moveSpeed.Normalize();

	moveSpeed *= MUL_STRAGHT_SPEED;

	moveSpeed *= m_magicBall->GetSpeedParameter()*g_gameTime->GetFrameDeltaTime();
	m_magicBall->SetMoveSpeed(moveSpeed);

	Vector3 currentPos = m_magicBall->GetPosition();

	if(setYUp) currentPos.y = yUp;

	currentPos += moveSpeed;

	m_magicBall->SetPosition(currentPos);

}

void MagicBallMovement::MoveFall()
{
	//�O�������擾
	Vector3 moveSpeed = g_vec3Down;

	moveSpeed *= MUL_STRAGHT_SPEED;

	moveSpeed *= m_magicBall->GetSpeedParameter() * g_gameTime->GetFrameDeltaTime();
	m_magicBall->SetMoveSpeed(moveSpeed);

	Vector3 currentPos = m_magicBall->GetPosition();

	currentPos += moveSpeed;

	m_magicBall->SetPosition(currentPos);
}

void MagicBallMovement::MoveChase(
	Vector3 targetPosition, float radius, float yUp, bool setYUp)
{

	// �v���C���[�̈ʒu�Ɍ������x�N�g��
	Vector3 directionToPlayer = targetPosition - m_magicBall->GetPosition();
	//�������v�Z
	float length = directionToPlayer.Length();
	// ���݂̒e�̈ړ��x�N�g��
	Vector3 currentDirection = m_magicBall->GetMoveSpeed();
	currentDirection.Normalize();

	//�x�N�g���̒������͈͓���������
	if (length < radius || m_isNearPlayerDistance == true)
	{
		m_isNearPlayerDistance = true;
		//�ǔ������O�������g��
		currentDirection = m_magicBall->GetForward();
		directionToPlayer = m_magicBall->GetForward();
	}
	directionToPlayer.Normalize();

	// �v���C���[�����ƌ��݂̒e�̕����̕�� (�ǔ����x�𒲐�)
	float homingSpeed = 0.6f; // �ǔ��̑���
	Vector3 newDirection = 
		currentDirection * (1.0f - homingSpeed) + directionToPlayer * homingSpeed;

	newDirection.Normalize();

	Vector3 moveSpeed = 
		(newDirection * MUL_CHASE_SPEED) * m_magicBall->GetSpeedParameter() * g_gameTime->GetFrameDeltaTime();

	// �V�������x�x�N�g����ݒ�
	//�ړ����x�ݒ�
	m_magicBall->SetMoveSpeed(moveSpeed);

	//�O������ݒ肷��
	moveSpeed.Normalize();
	m_magicBall->SetForward(moveSpeed);

	//
	Vector3 currentPos = m_magicBall->GetPosition();

	if (setYUp) currentPos.y = yUp;

	currentPos += m_magicBall->GetMoveSpeed();

	m_magicBall->SetPosition(currentPos);


}
