#include "stdafx.h"
#include "Result.h"
#include "Title.h"

namespace {
	const float WIPE_SIZE = 5.0f;
}

ResultSeen::ResultSeen()
{
}

ResultSeen::~ResultSeen()
{
}

bool ResultSeen::Start()
{
	m_resultSprite.Init("Assets/sprite/titleBack.DDS", 1920, 1080);
	m_resultSprite.SetRoundWipe(true);
	m_resultSprite.SetRoundWipeStartPosition(1920.0f / 2, 1080.0f / 2);
	m_resultSprite.SetWipeSize(m_wipeSize);
	m_resultSprite.Update();

	return true;
}

void ResultSeen::Update()
{
	if (m_roundWipeEndFlag != true)
	{
		m_resultSprite.SetWipeSize(m_wipeSize);
		m_wipeSize -= 7.0f;

		if (m_wipeSize < 0.0)
		{
			m_roundWipeEndFlag = true;
		}
	}
	//円形ワイプが終わったら
	else
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			Title* title = NewGO<Title>(0, "title");
			DeleteGO(this);
		}


	}


	
}

void ResultSeen::Render(RenderContext& rc)
{
	//m_resultSprite.Draw(rc);
}
