#include "stdafx.h"
#include "Meteo.h"

Meteo::Meteo()
{
}

Meteo::~Meteo()
{
	DeleteGO(m_collision);
}

bool Meteo::Start()
{
	m_model.Init("Assets/modelData/character/Slime/slime.tkm");

	//�n�_�����߂�
	m_startPosition = m_position;
	//���ԓ_�����߂�
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;
	//Y���W���グ��
	m_centerPosition.y += m_yUp;

	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	m_collision = NewGO<CollisionObject>(0, "meteo");
	m_collision->CreateSphere(
		m_position,
		m_rotation,
		30.0f
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();

	/*Vector3 diff = m_targetPosition - m_position;
	diff.Normalize();
	m_moveSpeed = diff * m_speed;*/


    return true;
}

void Meteo::Update()
{
	Move();
	Rotation();

	m_collision->SetPosition(m_movePos);
	m_collision->Update();
	m_model.SetTransform(m_movePos, m_rotation, m_scale);
	m_model.Update();

	m_position = m_movePos;
}

void Meteo::Move()
{
	//���`��Ԃ��g���Ĉړ�����
	//���_���璆�_
	StartToCenter.Lerp(m_timer, m_startPosition, m_centerPosition);
	//���_����I�_
	CenterToEnd.Lerp(m_timer, m_centerPosition, m_targetPosition);
	//���`��Ԃ����Q�̃x�N�g�����X�ɐ��`���
	m_movePos.Lerp(m_timer, StartToCenter, CenterToEnd);

	
	
	m_timer += 0.01f*0.2f;

	//m_model.SetPosition(m_movePos);
}

void Meteo::Rotation()
{
	//m_addRotation.AddRotationDegX(m_movePos.x);
	//m_addRotation.SetRotation(m_position, m_movePos);
	//m_rotation *= m_addRotation;

	//m_addRotation.AddRotationDegX(m_movePos.x);
	
	//m_rotation.AddRotationDegX(m_movePos.x);
}

void Meteo::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
