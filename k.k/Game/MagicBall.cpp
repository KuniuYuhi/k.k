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
    //移動方向に進行
    nowPosition += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
}

void MagicBall::MoveChase(Vector3 targetPosition)
{
}

void MagicBall::CalcMoveTime(const float moveLimitTime)
{
    //制限時間を超えたら
    if ( m_moveTimer > moveLimitTime)
    {
        //爆発処理
        Explosion();
    }
    else
    {
        //タイマーを加算
        m_moveTimer += g_gameTime->GetFrameDeltaTime();
    }
}

bool MagicBall::IsForceDelete(Vector3 currentPosition, float deleteLength)
{
    //スタート(生成)座標から現在の座標のベクトルを計算
    Vector3 diff = currentPosition - m_startPosition;

    //消去したい距離の長さを超えたら
    if (diff.Length() > deleteLength)
    {
        //消去できる
        return true;
    }

    //消去できない
    return false;
}

void MagicBall::Explosion()
{
}
