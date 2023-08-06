#include "stdafx.h"
#include "TurtleShell.h"

namespace {
	//ステータス
	int MAXHP = 100;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 70.0f;
	const char* NAME = "TurtleShell";
}

TurtleShell::TurtleShell()
{
}

TurtleShell::~TurtleShell()
{
}

bool TurtleShell::Start()
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

void TurtleShell::InitModel()
{
	m_modelRender.Init("Assets/modelData/character/TurtleShell/TurtleShell.tkm");

	m_position = { 100.0f, 0.0f, -200.0f };


	m_charaCon.Init(
		22.0f,
		4.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::Update()
{
	Move();
	Rotation();


	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::Move()
{
	SetTargetPosition();


	m_moveSpeed = calcVelocity(m_status);

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void TurtleShell::Damage(int attack)
{
}

bool TurtleShell::RotationOnly()
{
	return false;
}

void TurtleShell::PlayAnimation()
{
}

void TurtleShell::ManageState()
{
}

void TurtleShell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
