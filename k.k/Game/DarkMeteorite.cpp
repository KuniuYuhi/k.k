#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"

DarkMeteorite::DarkMeteorite()
{
}

DarkMeteorite::~DarkMeteorite()
{
}

bool DarkMeteorite::Start()
{
	m_model.Init("Assets/modelData/character/Slime/slime.tkm");


	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	return true;
}

void DarkMeteorite::Update()
{
	//一定のサイズになったら処理をしない
	SizeUp();
	
	Shot();


	m_model.Update();
}

void DarkMeteorite::SizeUp()
{
	//一定のサイズになったら処理をしない
	if (m_sizeUpFlag != false)
	{
		return;
	}

	m_scale *= 1.0f + g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetScale(m_scale);
	//一定のサイズになったら処理をしない
	if (m_maxScale.x <= m_scale.x)
	{
		m_sizeUpFlag = true;
	}
}

void DarkMeteorite::Shot()
{
	//ショットフラグが立ったら
	if (m_ShotStartFlag != true)
	{
		return;
	}

	//タイマーが生成時間に達していないなら抜け出す
	if (CreateTimer() != true)
	{
		return;
	}
	//生成する数
	if (m_meteoCounter < m_createMeteoCount)
	{
		//ターゲットを決めてメテオを生成
		Vector3 createPositon = m_targetPosition;

		float X = rand() % 1201 - 600;
		float Z = rand() % 1201 - 600;

		createPositon.x += X;
		createPositon.z += Z;

		CreateMeteo(createPositon);

		m_meteoCounter++;

		

	}
	//全てのメテオを生成したら
	else
	{
		m_shotEndFlag = true;
	}
	
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(m_position);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
}

bool DarkMeteorite::CreateTimer()
{
	if (m_createTime < m_createTimer)
	{
		m_createTimer = 0.0f;
		return true;
	}
	else
	{
		m_createTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
}

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
