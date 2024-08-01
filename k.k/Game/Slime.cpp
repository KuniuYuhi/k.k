#include "stdafx.h"
#include "Slime.h"

Slime::~Slime()
{
}

bool Slime::Start()
{

	//ƒ‚ƒfƒ‹‚ğ‰Šú‰»
	m_modelRender.Init(
		"Assets/modelData/character/Slime/slime.tkm",
		L"Assets/shader/ToonTextrue/lamp_Slime.DDS"
	);

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);


	m_status.InitCommonEnemyStatus("Slime");

	return true;
}

void Slime::Update()
{



	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	m_modelRender.Update();
}

void Slime::UpdateCharacter()
{
}

void Slime::SettingDefaultComponent()
{
}

void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
