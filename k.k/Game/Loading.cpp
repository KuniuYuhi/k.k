#include "stdafx.h"
#include "Loading.h"

#include "Title.h"
#include "Game.h"

namespace {
	const float TIMER_LIMMIT = 2.0f;
}

Loading::Loading()
{
}

Loading::~Loading()
{
}

bool Loading::Start()
{
	m_backSprite.Init(
		"Assets/sprite/Loading/loading.DDS", 1920.0f, 1080.0f
	);
	m_backSprite.SetPosition(g_vec3Zero);
	m_backSprite.Update();


	if (m_enLoadingRoot == enLoadingRoot_None)
	{
		return false;
	}

	return true;
}

void Loading::Update()
{
	//二秒経ったら転換

	if (m_seenChangeTimer > TIMER_LIMMIT)
	{
		//次のシーンを生成
		CreateNextSeen(m_enLoadingRoot);
		DeleteGO(this);
	}
	else
	{
		m_seenChangeTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void Loading::Render(RenderContext& rc)
{
	m_backSprite.Draw(rc);
}

void Loading::StartLoading(EnMethodLoading methodLoading)
{
	//方法の設定
	m_enMethodLoading = methodLoading;
	//ローディング始める
	m_enLoadingState = enLoadingState_In;
}

void Loading::EndLoading(EnMethodLoading methodLoading)
{
	//方法の設定
	m_enMethodLoading = methodLoading;
	//ローディング終わる
	m_enLoadingState = enLoadingState_Out;
}

void Loading::ManageState()
{
	switch (m_enLoadingState)
	{
	case Loading::enLoadingState_None:
		//何もしない
		break;
	case Loading::enLoadingState_In:
		ProcessInLoadingStateTransition();
		break;
	case Loading::enLoadingState_Out:
		ProcessOutLoadingStateTransition();
		break;
	default:
		break;
	}
}

void Loading::ProcessInLoadingStateTransition()
{
	switch (m_enMethodLoading)
	{
	case Loading::enMethodLoading_Fade:
		ProcessFade(m_enLoadingState);
		break;
	case Loading::enMethodLoading_DirectionWipe:
		ProcessDirectionWipe(m_enLoadingState);
		break;
	case Loading::enMethodLoading_RoundWipe:
		ProcessRoundWipe(m_enLoadingState);
		break;
	default:
		break;
	}
}

void Loading::ProcessOutLoadingStateTransition()
{
	switch (m_enMethodLoading)
	{
	case Loading::enMethodLoading_Fade:
		ProcessFade(m_enLoadingState);
		break;
	case Loading::enMethodLoading_DirectionWipe:
		ProcessDirectionWipe(m_enLoadingState);
		break;
	case Loading::enMethodLoading_RoundWipe:
		ProcessRoundWipe(m_enLoadingState);
		break;
	default:
		break;
	}
}

void Loading::ProcessFade(EnLoadingState loadingState)
{
	switch (loadingState)
	{
	case Loading::enLoadingState_In:
		
		break;
	case Loading::enLoadingState_Out:
		
		break;
	default:
		break;
	}
}

void Loading::ProcessDirectionWipe(EnLoadingState loadingState)
{
	switch (loadingState)
	{
	case Loading::enLoadingState_In:

		break;
	case Loading::enLoadingState_Out:

		break;
	default:
		break;
	}
}

void Loading::ProcessRoundWipe(EnLoadingState loadingState)
{
	switch (loadingState)
	{
	case Loading::enLoadingState_In:

		break;
	case Loading::enLoadingState_Out:

		break;
	default:
		break;
	}
}

void Loading::CreateNextSeen(EnLoadingRoot loadingRoot)
{
	switch (loadingRoot)
	{
	case Loading::enLoadingRoot_TitleToGame:
		CreateGame();
		break;
	case Loading::enLoadingRoot_GameToTitle:
		CreateTitle();
		break;
	default:std::abort();
		break;
	}
}

void Loading::CreateGame()
{
	Game* game = NewGO<Game>(0, "game");
}

void Loading::CreateTitle()
{
	Title* title = NewGO<Title>(0, "title");
}

void Loading::ProcessSpriteAlpha()
{
	switch (m_enLoadingState)
	{
	case enLoadingState_In:
		m_currentAlpha += m_fadeSpeed * g_gameTime->GetFrameDeltaTime();

		if (m_currentAlpha >= 1.0f)
		{
			m_currentAlpha = 1.0f;
			m_enLoadingState = enLoadingState_None;
		}

		break;

	case enLoadingState_Out:
		m_currentAlpha -= m_fadeSpeed * g_gameTime->GetFrameDeltaTime();

		if (m_currentAlpha <= 0.0f)
		{
			m_currentAlpha = 0.0f;
			m_enLoadingState = enLoadingState_None;
		}
		break;

	default:
		break;
	}

}
