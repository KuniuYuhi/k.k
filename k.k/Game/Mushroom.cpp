#include "stdafx.h"
#include "Mushroom.h"

namespace {
	//ステータス
	int MAXHP = 100;
	int MAXMP = 500;
	int ATK = 7;
	float SPEED = 90.0f;
	const char* NAME = "Mushroom";
}

Mushroom::Mushroom()
{
}

Mushroom::~Mushroom()
{
}

bool Mushroom::Start()
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

void Mushroom::InitModel()
{
	m_modelRender.Init("Assets/modelData/character/Mushroom/Mushroom.tkm");


	m_charaCon.Init(
		20.0f,
		5.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Mushroom::Update()
{
	Move();
	Rotation();


	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Mushroom::Move()
{
	SetTargetPosition();


	m_moveSpeed = calcVelocity(m_status);

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void Mushroom::Damage(int attack)
{
}

bool Mushroom::RotationOnly()
{
	return false;
}

void Mushroom::PlayAnimation()
{
}

void Mushroom::ManageState()
{
}

void Mushroom::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}