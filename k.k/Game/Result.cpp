#include "stdafx.h"
#include "Result.h"
#include "Title.h"

namespace {
	const float WIPE_SIZE = 5.0f;

	const Vector3 WIN_TEXT_POS = { 41.0f,346.0f,0.0f };
	const Vector3 RIGHT_POS = { 38.0f,442.0f,0.0f };

	const Vector3 LOSE_TEXT_POS = { 0.0f,363.0f,0.0f };

	const Vector3 GO_TO_TITLE_POS = { 0.0f,-407.0f,0.0f };

}

ResultSeen::ResultSeen()
{
}

ResultSeen::~ResultSeen()
{
}

bool ResultSeen::Start()
{
	m_resultSprite.Init("Assets/sprite/result.DDS", 1920, 1080);
	m_resultSprite.SetRoundWipe(true);
	m_resultSprite.SetRoundWipeStartPosition(1920.0f / 2, 1080.0f / 2);
	m_resultSprite.SetWipeSize(m_wipeSize);
	m_resultSprite.Update();


	InitSprite();

	return true;
}

void ResultSeen::Update()
{
	ManageState();
}

void ResultSeen::ManageState()
{
	switch (m_enOutComeState)
	{
	case ResultSeen::enOutcomeNone:
		//勝敗が分からなかったらクラッシュさせる
		std::abort;
		break;
	case ResultSeen::enOutcome_Win:
		WinState();
		break;
	case ResultSeen::enOutcome_Lose:
		LoseState();
		break;
	default:
		break;
	}

}

void ResultSeen::WinState()
{
	//
	if (m_resultEndFlag == true)
	{
		ResultEnd();
		return;
	}
	else
	{
		//円形ワイプを次フレームから開始
		//タイトルに戻るようにする
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_resultEndFlag = true;
		}
	}



	m_rightRotation.AddRotationDegZ(g_gameTime->GetFrameDeltaTime() * 20.0f);
	m_rightRender.SetRotation(m_rightRotation);
	m_rightRender.Update();
}

void ResultSeen::LoseState()
{
	//
	if (m_resultEndFlag == true)
	{
		ResultEnd();
		return;
	}
	else
	{
		//タイトルに戻るようにする
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_resultEndFlag = true;
		}
	}




	

}

void ResultSeen::ResultEnd()
{
	if (m_roundWipeEndFlag != true)
	{
		m_resultSprite.SetWipeSize(m_wipeSize);
		m_wipeSize -= 6.0f;

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
	switch (m_enOutComeState)
	{
	case ResultSeen::enOutcome_Win:
		m_rightRender.Draw(rc);
		m_winTextRender.Draw(rc);
		break;
	case ResultSeen::enOutcome_Lose:
		m_loseTextRender.Draw(rc);
		break;
	default:
		break;
	}

	m_goToTitleRender.Draw(rc);

	m_resultSprite.Draw(rc);
}

void ResultSeen::InitSprite()
{
	m_winTextRender.Init("Assets/sprite/InGame/Result_Win/Win!.DDS", 860, 340);
	SettingSpriteRender(m_winTextRender, WIN_TEXT_POS, g_vec3One, g_quatIdentity);

	m_rightRender.Init("Assets/sprite/InGame/Result_Win/light.DDS", 1410, 1410);
	SettingSpriteRender(m_rightRender, RIGHT_POS, g_vec3One, g_quatIdentity);

	m_loseTextRender.Init("Assets/sprite/InGame/Result_Lose/Lose_2.DDS", 1290, 265);
	SettingSpriteRender(m_loseTextRender, LOSE_TEXT_POS, g_vec3One, g_quatIdentity);

	m_goToTitleRender.Init("Assets/sprite/InGame/Result_Win/GoToTitle.DDS", 520, 70);
	SettingSpriteRender(m_goToTitleRender, GO_TO_TITLE_POS, g_vec3One, g_quatIdentity);


}
