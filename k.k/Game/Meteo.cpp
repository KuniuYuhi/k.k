#include "stdafx.h"
#include "Meteo.h"

Meteo::Meteo()
{
}

Meteo::~Meteo()
{
	DeleteGO(m_collision);
	DeleteGO(m_explosionCollision);
	
}

bool Meteo::Start()
{
	m_model.Init("Assets/modelData/character/Lich/Effect/Meteo.tkm");

	m_movePos = m_position;
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

	//�����͈͂̓����蔻��쐬(��A�N�e�B�u)
	m_explosionCollision = NewGO<CollisionObject>(0, "explosion");
	m_explosionCollision->CreateSphere(
		m_position,
		m_rotation,
		250.0f
	);
	m_explosionCollision->SetIsEnableAutoDelete(false);
	m_explosionCollision->SetPosition(m_position);
	m_explosionCollision->Update();
	//��A�N�e�B�u�ɂ���
	m_explosionCollision->Deactivate();

	/*Vector3 diff = m_targetPosition - m_position;
	diff.Normalize();
	m_moveSpeed = diff * m_speed;*/


    return true;
}

void Meteo::Update()
{
	if (GetExplosionFlag()==true)
	{
		Explosion();
		
	}
	else if(m_movePos.y <= 0.0f)
	{
		//�����͈͂̃R���W�������쐬
		//�A�N�e�B�u�ɂ���
		m_explosionCollision->Activate();
		m_explosionCollision->SetPosition(m_movePos);
		m_explosionCollision->Update();

		m_scale *= 12.0f;

		SetExplosionFlag(true);
	}
	else
	{
		Move();
		Rotation();
	}

	
	m_model.SetTransform(m_movePos, m_rotation, m_scale);
	m_model.Update();
	
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
	//
	m_timer += 0.01f*0.4f;

	//�ݒ�ƍX�V
	m_collision->SetPosition(m_movePos);
	m_collision->Update();
	

	m_position = m_movePos;
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

void Meteo::Explosion()
{
	if (m_explosionEndTime < m_explosionEndTimer)
	{
		DeleteGO(this);
	}
	else
	{
		m_explosionEndTimer += g_gameTime->GetFrameDeltaTime();
	}


	
	

	
}
