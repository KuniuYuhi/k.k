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
    m_model.Init("Assets/modelData/character/Slime/slime.tkm");


    m_position = m_wizard->GetPosition();
    m_rotation = m_wizard->GetRotation();

    Vector3 AddPos = Vector3::AxisZ;
    m_rotation.Apply(AddPos);
    //¶¬‚·‚éÀ•W‚ÌŒˆ’è
    m_position += AddPos * m_distance;

    m_model.SetTransform(
        m_position,
        m_rotation,
        g_vec3One
    );
    
    m_model.Update();

    Vector3 pillarPos = m_position;
    pillarPos.y += 30.0f;

    //“–‚½‚è”»’èì¬
    m_pillarCollision = NewGO<CollisionObject>(0, "flamepillar");
    m_pillarCollision->CreateCapsule(
        pillarPos,
        Quaternion::Identity,
        15.0f,
        40.0f
    );
    //“–‚½‚è”»’è‚Í‚È‚µ
    //m_pillarCollision->SetIsEnable(false);
    m_pillarCollision->SetIsEnableAutoDelete(false);
    m_pillarCollision->SetPosition(pillarPos);
    m_pillarCollision->Update();

    return true;
}

void FlamePillar::Update()
{
    Timer();
}

void FlamePillar::Timer()
{
    //§ŒÀŠÔ‚É’B‚µ‚½‚Æ‚«
    if (m_pillarLimitTime < m_pillarTimer)
    {
        DeleteGO(this);
    }
    else
    {
        m_pillarTimer += g_gameTime->GetFrameDeltaTime();
    }
}

void FlamePillar::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}
