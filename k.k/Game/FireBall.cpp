#include "stdafx.h"
#include "FireBall.h"
#include "InitEffect.h"

namespace {
    const float FIRE_BALL_SIZE = 6.0f;
    const float EXPLOSION_SIZE = 6.0f;
}

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
    //����������W�̌���
    m_position += m_moveSpeed * ADD_CREATE_POS;
    //���x�����߂�
    m_moveSpeed *= SPEED;


    //�����蔻��̐ݒ�
    SettingCollision();

    //�G�t�F�N�g�̐ݒ�
    m_fireBallEffect = NewGO<EffectEmitter>(0, "fireball");
    m_fireBallEffect->Init(InitEffect::enEffect_FireBall);
    m_fireBallEffect->Play();
    m_fireBallEffect->SetScale(g_vec3One * FIRE_BALL_SIZE);
    m_fireBallEffect->SetPosition(m_collisionPosition);
    m_fireBallEffect->SetRotation(m_rotation);
    m_fireBallEffect->Update();

	return true;
}

void FireBall::Update()
{
    //�G�ɂԂ������玩�g������
    if (m_hitFlag == true)
    {
        Explosion();
    }

    CalcMoveTime(m_moveLimitTimer);

    Move();

}

void FireBall::Move()
{
    MoveStraight();

    m_collisionPosition = m_position;
    m_collisionPosition.y += Y_UP;

    //�G�t�F�N�g�̈ړ�
    m_fireBallEffect->SetPosition(m_collisionPosition);
    m_fireBallEffect->Update();
    //�����蔻��̈ړ�
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}

void FireBall::SettingCollision()
{ 
    //�����蔻��̍��W�̐ݒ�
    m_collisionPosition = m_position;

    //�����蔻��쐬
    m_BallCollision = NewGO<CollisionObject>(0, "fireball");
    m_BallCollision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        RADIUS
    );
    //���蔲����悤�ɂ���
    m_BallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y += Y_UP;
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}

void FireBall::Explosion()
{
    //����
    EffectEmitter* explosionEffect = NewGO<EffectEmitter>(0);
    explosionEffect->Init(InitEffect::enEffect_FireBall_Explosion);
    explosionEffect->Play();
    explosionEffect->SetScale(g_vec3One * EXPLOSION_SIZE);
    explosionEffect->SetPosition(m_collisionPosition);
    explosionEffect->Update();

    DeleteGO(this);
}