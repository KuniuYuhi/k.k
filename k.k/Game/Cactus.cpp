#include "stdafx.h"
#include "Cactus.h"

namespace {
	//ステータス
	int MAXHP = 170;
	int MAXMP = 500;
	int ATK = 10;
	float SPEED = 85.0f;
	const char* NAME = "Cactus";
}

Cactus::Cactus()
{
}

Cactus::~Cactus()
{
}

bool Cactus::Start()
{
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	return true;
}

void Cactus::InitModel()
{
	m_modelRender.Init("Assets/modelData/character/Cactus/Cactus.tkm");

	m_position = { -100.0f, 0.0f, -200.0f };


	m_charaCon.Init(
		20.0f,
		5.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Update()
{
	Move();
	Rotation();


	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Move()
{
	SetTargetPosition();


	m_moveSpeed = calcVelocity(m_status);

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void Cactus::Damage(int attack)
{
}

bool Cactus::RotationOnly()
{
	return false;
}

void Cactus::PlayAnimation()
{
}

void Cactus::ManageState()
{
}

void Cactus::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}