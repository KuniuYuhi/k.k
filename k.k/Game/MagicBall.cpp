#include "stdafx.h"
#include "MagicBall.h"

MagicBall::MagicBall()
{
}

MagicBall::~MagicBall()
{
}

void MagicBall::MoveStraight()
{
    m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
}

void MagicBall::MoveChase(Vector3 targetPosition)
{
}

void MagicBall::CalcMoveTime(const float moveLimitTime)
{
    if (moveLimitTime < m_moveTimer)
    {
        Explosion();
    }
    else
    {
        m_moveTimer += g_gameTime->GetFrameDeltaTime();
    }
}

void MagicBall::Explosion()
{
}
