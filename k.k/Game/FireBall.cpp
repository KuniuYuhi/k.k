#include "stdafx.h"
#include "FireBall.h"
#include "InitEffect.h"

namespace {
    const int ADD_CREATE_POS = 60;
    const float SPEED = 180.0f;
    const float RADIUS = 15.0f;
    const float Y_UP = 20.0f;


}

FireBall::FireBall()
{
}

FireBall::~FireBall()
{
    if (m_BallCollision != nullptr)
    {
        DeleteGO(m_BallCollision);
    }

    if (m_fireBallEffect != nullptr)
    {
        m_fireBallEffect->Stop();
    }
}

bool FireBall::Start()
{
    SetMoveSpeed();
    //生成する座標の決定
    m_position += m_moveSpeed * ADD_CREATE_POS;
    //速度を決める
    m_moveSpeed *= SPEED;


    //当たり判定の設定
    SettingCollision();

    //エフェクトの設定
    m_fireBallEffect = NewGO<EffectEmitter>(0, "fireball");
    m_fireBallEffect->Init(InitEffect::enEffect_DarkBall);
    m_fireBallEffect->Play();
    m_fireBallEffect->SetScale({ 6.0f,6.0f,6.0f });
    m_fireBallEffect->SetPosition(m_collisionPosition);
    m_fireBallEffect->SetRotation(m_rotation);
    m_fireBallEffect->Update();

	return true;
}

void FireBall::Update()
{
    //敵にぶつかったら自身を消す
    if (m_hitFlag == true)
    {
        DeleteGO(this);
    }

    CalcMoveTime(m_moveLimitTimer);

    Move();

}

void FireBall::Move()
{
    MoveStraight();

    m_collisionPosition = m_position;
    m_collisionPosition.y += Y_UP;

    //エフェクトの移動
    m_fireBallEffect->SetPosition(m_collisionPosition);
    m_fireBallEffect->Update();
    //当たり判定の移動
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}

void FireBall::SettingCollision()
{ 
    //当たり判定の座標の設定
    m_collisionPosition = m_position;

    //当たり判定作成
    m_BallCollision = NewGO<CollisionObject>(0, "fireball");
    m_BallCollision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        RADIUS
    );
    //すり抜けるようにする
    m_BallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y += Y_UP;
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}

void FireBall::Explosion()
{
    DeleteGO(this);
}