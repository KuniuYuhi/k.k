#include "stdafx.h"
#include "FlamePillar.h"
#include "Wizard.h"

FlamePillar::FlamePillar()
{
}

FlamePillar::~FlamePillar()
{
}

bool FlamePillar::Start()
{
    m_model.Init("Assets/modelData/character/Slime/slime.tkm");


    m_position = m_wizard->GetPosition();
    m_rotation = m_wizard->GetRotation();

    Vector3 AddPos = Vector3::AxisZ;
    m_rotation.Apply(AddPos);
    //ê∂ê¨Ç∑ÇÈç¿ïWÇÃåàíË
    m_position += AddPos * m_distance;

    m_model.SetTransform(
        m_position,
        m_rotation,
        g_vec3One
    );
    
    m_model.Update();

    //m_rotation.AddRotationDegY(360.0f);

    return true;
}

void FlamePillar::Update()
{
    Timer();
}

void FlamePillar::Timer()
{
    //êßå¿éûä‘Ç…íBÇµÇΩÇ∆Ç´
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
