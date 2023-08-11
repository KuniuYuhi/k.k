#include "stdafx.h"
#include "DarkWall.h"
#include "Lich.h"

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

    //m_position.y -= 50.0f;

    //�{�[���̍��W�̎擾
    Matrix matrix = m_lich->GetModelRender().GetBone(m_lich->GetDarkWallBoonId())->GetWorldMatrix();

    

    //�x�N�g���ɍs�����Z
    matrix.Apply(m_position);

    
    
    m_rotation = m_lich->GetRotation();

    m_moveSpeed = Vector3::AxisZ;
    m_rotation.Apply(m_moveSpeed);

    ////����������W�̌���
    //m_position += m_moveSpeed;
    ////���x�����߂�
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
    
    m_model.SetTransform(m_position, m_rotation, g_vec3One);
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
