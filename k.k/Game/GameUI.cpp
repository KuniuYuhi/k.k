#include "stdafx.h"
#include <codecvt>
#include "GameUI.h"

#include "PlayerGameUI.h"
#include "SummonerUI.h"
#include "GameStatusUI.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"




GameUI::GameUI()
{
}

GameUI::~GameUI()
{
	m_playerGameUI->DeleteSelf();
	m_summonerUI->DeleteSelf();
	m_gameStatusUI->DeleteSelf();
}

bool GameUI::Start()
{

	m_playerGameUI = NewGO<PlayerGameUI>(0, "PlayerGameUI");

	m_summonerUI = NewGO<SummonerUI>(0, "SummonerUI");

	m_gameStatusUI = NewGO<GameStatusUI>(0, "GameStatusUI");

	return true;
}

void GameUI::Update()
{
	//Ÿ”s‚ª’…‚¢‚½‚çíœ‚·‚é
	if (GameSceneManager::GetInstance()->GetBattleOutCome() != 
		GameSceneManager::enBattleOutCome_None)
	{
		DeleteGO(this);
		return;
	}


	m_playerGameUI->UIUpdate();

	m_summonerUI->UIUpdate();

	m_gameStatusUI->UIUpdate();
}

void GameUI::Render(RenderContext& rc)
{
	m_playerGameUI->Draw(rc);
	m_summonerUI->Draw(rc);
	m_gameStatusUI->Draw(rc);
}