#include "stdafx.h"
#include "Loading.h"

#include "Title.h"
#include "Game.h"

#include "GameSceneManager.h"


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


	for (int i = 0; i < 3; i++)
	{
		m_dotSprite[i].Init("Assets/sprite/Loading/Dot.DDS", 20.0f, 20.0f);
		m_dotSprite[i].SetPosition(m_dotPositions[i]);
		m_dotSprite[i].Update();
	}


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
		if (m_dotCountTimer >= 0.3f)
		{
			m_currentDotCount++;
			if (m_currentDotCount > 3)
			{
				m_currentDotCount = 0;
			}


			m_dotCountTimer = 0.0f;
		}
		else
		{
			m_dotCountTimer += g_gameTime->GetFrameDeltaTime();
		}


		m_seenChangeTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void Loading::Render(RenderContext& rc)
{
	m_backSprite.Draw(rc);


	for (int i = 0; i < m_currentDotCount; i++)
	{
		m_dotSprite[i].Draw(rc);
	}

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
	//シーン切り替え可能かフラグをセット
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(true);
}

void Loading::CreateTitle()
{
	//シーン切り替え可能かフラグをセット
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(true);
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
