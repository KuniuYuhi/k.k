#include "stdafx.h"
#include "FlamePillar.h"
#include "Wizard.h"

FlamePillar::FlamePillar()
{
}

FlamePillar::~FlamePillar()
{
    DeleteGO(m_pillarCollision);
}

bool FlamePillar::Start()
{
    m_model.Init("Assets/modelData/character/Wizard/Effect/FireBall.tkm");


    m_position = m_wizard->GetPosition();
    m_rotation = m_wizard->GetRotation();

    Vector3 AddPos = Vector3::AxisZ;
    m_rotation.Apply(AddPos);
    //����������W�̌���
    m_position += AddPos * m_distance;

    m_model.SetTransform(
        m_position,
        m_rotation,
        g_vec3One
    );
    
    m_model.Update();

    Vector3 pillarPos = m_position;
    pillarPos.y += 30.0f;

    //�����蔻��쐬
    m_pillarCollision = NewGO<CollisionObject>(0, "flamepillar");
    m_pillarCollision->CreateCapsule(
        pillarPos,
        Quaternion::Identity,
        15.0f,
        40.0f
    );
    //�����蔻��͂Ȃ�
    //m_pillarCollision->SetIsEnable(false);
    m_pillarCollision->SetIsEnableAutoDelete(false);
    m_pillarCollision->SetPosition(pillarPos);
    m_pillarCollision->Update();

    return true;
}

void FlamePillar::Update()
{
    CalcDamageTime();
    Timer();
}

void FlamePillar::Timer()
{
    //�������ԂɒB�����Ƃ�
    if (m_pillarLimitTime < m_pillarTimer)
    {
        DeleteGO(this);
    }
    else
    {
        m_pillarTimer += g_gameTime->GetFrameDeltaTime();
    }
}

void FlamePillar::CalcDamageTime()
{
    if (m_canDamageFlag != true)
    {
        return;
    }

    if (m_canDamageTime < m_canDamageTimer)
    {
        m_canDamageTimer = 0.0f;
        m_canDamageFlag = false;
    }
    else
    {
        m_canDamageTimer += g_gameTime->GetFrameDeltaTime();
    }
}

void FlamePillar::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}
