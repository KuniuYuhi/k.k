#include "stdafx.h"
#include "Meteo.h"

//todo ���e�I����������Ƃ��������~�܂�

namespace {
	const float EXPLOSION_SCALE = 12.0f;
	const Vector3 SCALE = { 1.4f,1.4f,1.4f };
}

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
	m_model.Init("Assets/modelData/character/Lich/Effect/Meteo.tkm", nullptr, 0, enModelUpAxisZ, false, false, false);

	m_movePos = m_position;
	//�n�_�����߂�
	m_startPosition = m_position;
	//���ԓ_�����߂�
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;
	//Y���W���グ��
	m_centerPosition.y += m_yUp;

	m_scale = SCALE;

	m_model.SetTransform(m_position, m_rotation, m_scale);
	m_model.Update();
	//���e�I�̓����蔻��̐���
	m_collision = NewGO<CollisionObject>(0, "meteo");
	m_collision->CreateSphere(
		m_position,
		m_rotation,
		30.0f
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();

    return true;
}

void Meteo::Update()
{
	//����������
	if (GetExplosionFlag()==true)
	{
		CalcDeleteTime();
		
	}
	else if(m_movePos.y <= 0.0f)
	{
		DeleteGO(m_collision);
		CreateExplosionCollision();
		m_scale *= EXPLOSION_SCALE;
		//��������
		SetExplosionFlag(true);
	}
	else
	{
		Move();
	}

	m_model.SetTransform(m_position, m_rotation, m_scale);
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
	m_timer += 0.01f*0.65f;

	//�ݒ�ƍX�V
	m_collision->SetPosition(m_movePos);
	m_collision->Update();
	
	m_position = m_movePos;
}

void Meteo::CreateExplosionCollision()
{
	//�����͈͂̓����蔻��쐬
	auto explosionCollision = NewGO<CollisionObject>(0, "explosion");
	explosionCollision->CreateSphere(
		m_position,
		m_rotation,
		280.0f
	);
	explosionCollision->SetPosition(m_position);
}

void Meteo::Explosion()
{
	//�����R���W�����͍쐬���Ȃ�
	//��������
	SetExplosionFlag(true);
	m_scale *= EXPLOSION_SCALE;
}

void Meteo::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void Meteo::CalcDeleteTime()
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
