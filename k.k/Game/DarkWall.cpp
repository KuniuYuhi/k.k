#include "stdafx.h"
#include "DarkWall.h"
#include "Lich.h"

namespace {
    const Vector3 SCALE = { 2.0f,2.0f,2.0f };
}

DarkWall::DarkWall()
{
}

DarkWall::~DarkWall()
{
    DeleteGO(m_collision);
}

bool DarkWall::Start()
{
    m_model.Init("Assets/modelData/character/Lich/Effect/Meteo.tkm");

    //攻撃力を設定
    m_attak = m_lich->GetStatus().atk;

    //ボーンの座標の取得
    Matrix matrix = m_lich->GetModelRender().GetBone(m_lich->GetDarkWallBoonId())->GetWorldMatrix();
    //ベクトルに行列を乗算
    matrix.Apply(m_position);
    m_rotation = m_lich->GetRotation();

    m_moveSpeed = Vector3::AxisZ;
    m_rotation.Apply(m_moveSpeed);

    ////生成する座標の決定
    //m_position += m_moveSpeed;
    ////速度を決める
    //m_moveSpeed *= m_speed;
    m_collision = NewGO<CollisionObject>(0, "DarkWall");
    m_collision->CreateSphere(
        m_position,
        Quaternion::Identity,
        50.0f
    );
    m_collision->SetIsEnableAutoDelete(false);
    m_collision->SetPosition(m_position);
    m_collision->Update();
    
    m_model.SetTransform(m_position, m_rotation, SCALE);
    m_model.Update();

    return true;
}

void DarkWall::Update()
{
    if (m_deleteTime < m_deleteTimer)
    {
        DeleteGO(this);
    }
    else
    {
        m_deleteTimer += g_gameTime->GetFrameDeltaTime();
    }

    m_position.y -= 3.0f;

    m_collision->SetPosition(m_position);
    m_collision->Update();
    m_model.SetPosition(m_position);
    m_model.Update();
}

void DarkWall::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}
