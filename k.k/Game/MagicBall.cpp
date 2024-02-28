#include "stdafx.h"
#include "MagicBall.h"

MagicBall::MagicBall()
{
}

MagicBall::~MagicBall()
{
}

void MagicBall::MoveStraight(Vector3& nowPosition)
{
    //�ړ������ɐi�s
    nowPosition += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
}

void MagicBall::MoveChase(Vector3 targetPosition)
{
}

void MagicBall::CalcMoveTime(const float moveLimitTime)
{
    //�������Ԃ𒴂�����
    if ( m_moveTimer > moveLimitTime)
    {
        //��������
        Explosion();
    }
    else
    {
        //�^�C�}�[�����Z
        m_moveTimer += g_gameTime->GetFrameDeltaTime();
    }
}

bool MagicBall::IsForceDelete(Vector3 currentPosition, float deleteLength)
{
    //�X�^�[�g(����)���W���猻�݂̍��W�̃x�N�g�����v�Z
    Vector3 diff = currentPosition - m_startPosition;

    //���������������̒����𒴂�����
    if (diff.Length() > deleteLength)
    {
        //�����ł���
        return true;
    }

    //�����ł��Ȃ�
    return false;
}

void MagicBall::Explosion()
{
}
