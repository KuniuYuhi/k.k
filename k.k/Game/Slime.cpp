#include "stdafx.h"
#include "Slime.h"

namespace {
	//ステータス
	int MAXHP = 150;
	int MAXMP = 500;
	int ATK = 10;
	float SPEED = 80.0f;
	const char* NAME = "Slime";
}

Slime::Slime()
{
}

Slime::~Slime()
{
}

bool Slime::Start()
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

void Slime::InitModel()
{
	m_modelRender.Init("Assets/modelData/character/Slime/slime.tkm");

	m_position = { 0.0f, 0.0f, -200.0f };


	m_charaCon.Init(
		18.0f,
		4.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Slime::Update()
{

	Move();
	Rotation();


	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Slime::Move()
{
	SetTargetPosition();


	m_moveSpeed = calcVelocity(m_status);

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void Slime::Damage(int attack)
{
}

bool Slime::RotationOnly()
{
	return false;
}

void Slime::ManageState()
{
}

void Slime::PlayAnimation()
{
	
}



void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
