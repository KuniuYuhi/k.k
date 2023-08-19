#include "stdafx.h"
#include "EntryBoss.h"
#include "Game.h"

namespace {
	const float SCALE_UP = 3.0f;
}

EntryBoss::EntryBoss()
{
}

EntryBoss::~EntryBoss()
{
}

bool EntryBoss::Start()
{

	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);

	m_model.Init(
		"Assets/modelData/character/Lich/Lich.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	m_position.y -= 300.0f;
	m_scale *= SCALE_UP;

	m_rotation.SetRotationDegY(180.0f);
	m_model.SetTransform(m_position, m_rotation, m_scale);
	m_model.Update();

	Vector3 pointLightPos = m_position;
	pointLightPos.y = 200.0f;
	//ポイントライト
	/*g_renderingEngine->SetPointLight(
		pointLightPos,
		Vector3(15.0f, 0.0f, 12.0f),
		Vector3(700.0f, 3.0f, 0.0f)
	);*/

	/*g_renderingEngine->SetSpotLight(
		pointLightPos,
		Vector3(100.0f,0.0f,100.0f),
		Vector3(500.0f, 10.0f, 0.0f),
		Vector3(1.0f,-1.0f,0.0f),
		Vector3(90.0f, 40.0f, 0.0f)
	);*/

	return true;
}

void EntryBoss::Update()
{
	positionUp();


	m_model.PlayAnimation(enAnimClip_Idle);
	m_model.Update();
}

void EntryBoss::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void EntryBoss::positionUp()
{
	if (m_position.y >= 0.0f)
	{
		m_position.y = 0.0f;
		m_game->SetBossMovieFlag(true);
	}
	else
	{
		m_position.y += g_gameTime->GetFrameDeltaTime() * 35.0f;
	}

	m_model.SetPosition(m_position);

}
