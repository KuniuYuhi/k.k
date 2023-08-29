#include "stdafx.h"
#include "Meteo.h"

namespace {
	const float EXPLOSION_SCALE = 12.0f;
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
	m_model.Init("Assets/modelData/character/Lich/Effect/Meteo.tkm");

	m_movePos = m_position;
	//始点を決める
	m_startPosition = m_position;
	//中間点を決める
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;
	//Y座標を上げる
	m_centerPosition.y += m_yUp;

	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();


	//
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
	//爆発したら
	if (GetExplosionFlag()==true)
	{
		CalcDeleteTime();
		
	}
	else if(m_movePos.y <= 0.0f)
	{
		DeleteGO(m_collision);
		CreateExplosionCollision();
		m_scale *= EXPLOSION_SCALE;
		//爆発した
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
	//線形補間を使って移動する
	//視点から中点
	StartToCenter.Lerp(m_timer, m_startPosition, m_centerPosition);
	//中点から終点
	CenterToEnd.Lerp(m_timer, m_centerPosition, m_targetPosition);
	//線形補間した２つのベクトルを更に線形補間
	m_movePos.Lerp(m_timer, StartToCenter, CenterToEnd);
	//
	m_timer += 0.01f*0.4f;

	//設定と更新
	m_collision->SetPosition(m_movePos);
	m_collision->Update();
	

	m_position = m_movePos;
}

void Meteo::CreateExplosionCollision()
{
	//爆発範囲の当たり判定作成(非アクティブ)
	auto explosionCollision = NewGO<CollisionObject>(0, "explosion");
	explosionCollision->CreateSphere(
		m_position,
		m_rotation,
		250.0f
	);
	explosionCollision->SetPosition(m_position);
}

void Meteo::Explosion()
{
	//爆発コリジョンは作成しない
	//爆発した
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
