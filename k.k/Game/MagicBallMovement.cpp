#include "stdafx.h"
#include "MagicBallMovement.h"
#include "IMagicBall.h"


namespace {
	float MUL_SPEED = 10.0f;
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

void MagicBallMovement::MoveStraight()
{
	//‘O•ûŒü‚ðŽæ“¾
	Vector3 moveSpeed = m_magicBall->GetForward();
	moveSpeed.Normalize();

	moveSpeed *= MUL_SPEED;

	moveSpeed *= m_magicBall->GetSpeedParameter()*g_gameTime->GetFrameDeltaTime();
	m_magicBall->SetMoveSpeed(moveSpeed);

	Vector3 currentPos = m_magicBall->GetPosition();

	currentPos += moveSpeed;

	m_magicBall->SetPosition(currentPos);

}

void MagicBallMovement::MoveFall()
{
	//‘O•ûŒü‚ðŽæ“¾
	Vector3 moveSpeed = g_vec3Down;

	moveSpeed *= MUL_SPEED;

	moveSpeed *= m_magicBall->GetSpeedParameter() * g_gameTime->GetFrameDeltaTime();
	m_magicBall->SetMoveSpeed(moveSpeed);

	Vector3 currentPos = m_magicBall->GetPosition();

	currentPos += moveSpeed;

	m_magicBall->SetPosition(currentPos);
}

void MagicBallMovement::MoveChase(Vector3 targetPosition)
{
}
