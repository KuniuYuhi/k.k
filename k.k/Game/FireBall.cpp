#include "stdafx.h"
#include "FireBall.h"
#include "Wizard.h"

FireBall::FireBall()
{
}

FireBall::~FireBall()
{
    if (m_fireBallCollision != nullptr)
    {
        DeleteGO(m_fireBallCollision);
    }
}

bool FireBall::Start()
{
    m_model.Init("Assets/modelData/character/Slime/slime.tkm");


    m_position = m_wizard->GetPosition();
    m_rotation = m_wizard->GetRotation();

    m_moveSpeed = Vector3::AxisZ;
    m_rotation.Apply(m_moveSpeed);
    //生成する座標の決定
    m_position += m_moveSpeed * m_distance;
    //速度を決める
    m_moveSpeed *= m_speed;

    m_model.SetTransform(
        m_position,
        m_rotation,
        g_vec3One
    );

    m_model.Update();

    //当たり判定の座標の設定
    m_collisionPosition = m_position;

    //当たり判定作成
    m_fireBallCollision = NewGO<CollisionObject>(0, "fireball");
    m_fireBallCollision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        15.0f
    );
    //すり抜けるようにする
    //m_fireBallCollision->SetIsEnable(false);
    m_fireBallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y += 20.0f;
    m_fireBallCollision->SetPosition(m_collisionPosition);
    m_fireBallCollision->Update();
    
	return true;
}

void FireBall::Update()
{
    //敵にぶつかったら自身を消す
    if (m_hitEnemeyFlag == true)
    {
        DeleteGO(this);
    }

    Timer();

    Move();

}

void FireBall::Timer()
{
    if (m_limitTimer < m_moveTime)
    {
        DeleteGO(this);
    }
    else
    {
        m_moveTime += g_gameTime->GetFrameDeltaTime();
    }
}

void FireBall::Move()
{
    m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
    m_collisionPosition = m_position;
    m_collisionPosition.y += 20.0f;

    m_model.SetPosition(m_position);
    m_model.Update();
    m_fireBallCollision->SetPosition(m_collisionPosition);
    m_fireBallCollision->Update();
}

void FireBall::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}
