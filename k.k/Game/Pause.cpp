#include "stdafx.h"
#include "Pause.h"

Pause::Pause()
{
}

Pause::~Pause()
{
}

bool Pause::Start()
{
	InitSprite();
	return true;
}

void Pause::Update()
{
	Select();
}



void Pause::InitSprite()
{
	m_bgRender.Init(
		"Assets/sprite/InGame/Result_Lose/Fade_Black.DDS", 1920, 1080);
	m_bgRender.SetMulColor({ 1.0f,1.0f,1.0f,0.4f });
	m_bgRender.Update();

}

void Pause::Select()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_gameExitFlag = true;
	}
}


void Pause::Render(RenderContext& rc)
{
	m_bgRender.Draw(rc);
}