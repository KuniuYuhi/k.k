#include "stdafx.h"
#include "FireBall.h"
#include "Wizard.h"

FireBall::FireBall()
{
}

FireBall::~FireBall()
{
}

bool FireBall::Start()
{
    m_model.Init("Assets/modelData/character/Slime/slime.tkm");


    m_position = m_wizard->GetPosition();
    m_rotation = m_wizard->GetRotation();

    m_moveSpeed = Vector3::AxisZ;
    m_rotation.Apply(m_moveSpeed);
    //ê∂ê¨Ç∑ÇÈç¿ïWÇÃåàíË
    m_position += m_moveSpeed * m_distance;
    //ë¨ìxÇåàÇﬂÇÈ
    m_moveSpeed *= m_speed;

    m_model.SetTransform(
        m_position,
        m_rotation,
        g_vec3One
    );

    m_model.Update();

	return true;
}

void FireBall::Update()
{
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


    m_model.SetPosition(m_position);

    m_model.Update();
}

void FireBall::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}
