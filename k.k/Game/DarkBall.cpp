#include "stdafx.h"
#include "DarkBall.h"
#include "InitEffect.h"

namespace {
    const int ADD_CREATE_POS = 100;
    const float SPEED = 350.0f;
    const float RADIUS = 45.0f;
    const float Y_UP = 60.0f;

    const float EXPLOSION_SIZE = 9.0f;

    const float DARK_BALL_SIZE = 12.0f;

}

DarkBall::DarkBall()
{
    m_speed = SPEED;
}

DarkBall::~DarkBall()
{
    if (m_BallCollision != nullptr)
    {
        DeleteGO(m_BallCollision);
    }

    if (m_darkBallEffect != nullptr)
    {
        m_darkBallEffect->Stop();
    }
}

bool DarkBall::Start()
{
    SetMoveSpeed();
    //生成する座標の決定
    m_position += m_moveSpeed * ADD_CREATE_POS;
    //速度を決める
    m_moveSpeed *= SPEED;

    //当たり判定の設定
    SettingCollision();

    //エフェクトの設定
    m_darkBallEffect = NewGO<EffectEmitter>(0,"DarkBall");
    m_darkBallEffect->Init(InitEffect::enEffect_DarkBall);
    m_darkBallEffect->Play();
    m_darkBallEffect->SetScale(g_vec3One * DARK_BALL_SIZE);
    m_darkBallEffect->SetPosition(m_collisionPosition);
    m_darkBallEffect->SetRotation(m_rotation);
    m_darkBallEffect->Update();

    g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_DarkBall, g_soundManager->GetSEVolume());

	return true;
}

void DarkBall::Update()
{
    if (m_hitFlag == true)
    {
        Explosion();
    }

    CalcMoveTime(m_moveLimitTimer);

    Move();

}

void DarkBall::Move()
{
    MoveStraight();
  
    //当たり判定の移動
    m_collisionPosition = m_position;
    m_collisionPosition.y += Y_UP;

    //エフェクトの移動
    m_darkBallEffect->SetPosition(m_collisionPosition);
    m_darkBallEffect->Update();
    //当たり判定の移動
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}

void DarkBall::SettingCollision()
{
    //当たり判定の座標の設定
    m_collisionPosition = m_position;

    //当たり判定作成
    m_BallCollision = NewGO<CollisionObject>(0, "darkball");
    m_BallCollision->SetCreatorName(GetName());
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

void DarkBall::Explosion()
{
    m_darkBallEffect->Stop();

    g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_DarkBall_Explosion, g_soundManager->GetSEVolume());

    //爆発
    EffectEmitter* explosionEffect = NewGO<EffectEmitter>(0);
    explosionEffect->Init(InitEffect::enEffect_DarkBall_Explosion);
    explosionEffect->Play();
    explosionEffect->SetScale(g_vec3One * EXPLOSION_SIZE);
    explosionEffect->SetPosition(m_collisionPosition);
    explosionEffect->Update();

    DeleteGO(this);
}
