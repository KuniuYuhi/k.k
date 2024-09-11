#include "stdafx.h"
#include <codecvt>
#include "GameUI.h"

#include "PlayerGameUI.h"
#include "SummonerUI.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"



GameUI::GameUI()
{
}

GameUI::~GameUI()
{
	DeleteGO(m_playerGameUI);
}

bool GameUI::Start()
{

	m_playerGameUI = NewGO<PlayerGameUI>(0, "PlayerGameUI");

	m_summonerUI = NewGO<SummonerUI>(0, "SummonerUI");

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
}

void GameUI::Render(RenderContext& rc)
{
	m_playerGameUI->Draw(rc);
	m_summonerUI->Draw(rc);
}