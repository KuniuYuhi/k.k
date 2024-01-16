#include "stdafx.h"
#include "Loading.h"

Loading::Loading()
{
}

Loading::~Loading()
{
}

bool Loading::Start()
{
	m_backSprite.Init(
		"Assets/sprite/Fade/Fade_Black.DDS", 1920.0f, 1080.0f
	);
	m_backSprite.SetPosition(g_vec3Zero);
	m_backSprite.Update();

	return true;
}

void Loading::Update()
{

}

void Loading::Render(RenderContext& rc)
{
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
