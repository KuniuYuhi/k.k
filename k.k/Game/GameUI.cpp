#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Player.h"
#include "Lich.h"

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

bool GameUI::Start()
{
	//プレイヤーのUI
	InitPlayerUI();
	//モンスターのUI
	InitMonsterUI();


	return true;
}

void GameUI::Update()
{

	PlayerUIUpdate();

	MonsterUIUpdate();


}

void GameUI::PlayerUIUpdate()
{
	//MPの表示
	int NowActorMP = m_player->GetNowActorStatus().mp;
	int NowActorMaxMP = m_player->GetNowActorStatus().maxMp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"MP %3d/%d", NowActorMP, NowActorMaxMP);
	m_playerUI.m_mpFont.SetText(MP);

	int NowActorHP = m_player->GetNowActorStatus().hp;
	int NowActorMaxHP = m_player->GetNowActorStatus().maxHp;
	wchar_t HP[255];
	swprintf_s(HP, 255, L"HP %3d/%d", NowActorHP, NowActorMaxHP);
	m_playerUI.m_hpFont.SetText(HP);


}

void GameUI::MonsterUIUpdate()
{
	//MPの表示
	int NowActorMP = m_lich->GetStatus().hp;
	int NowActorMaxMP = m_lich->GetStatus().maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"ボス HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_monsterUI.m_hpFont.SetText(MP);

}

void GameUI::DrawPlayerUI(RenderContext& rc)
{
	m_playerUI.m_hpFont.Draw(rc);
	m_playerUI.m_mpFont.Draw(rc);
}

void GameUI::DrawMonsterUI(RenderContext& rc)
{
	m_monsterUI.m_hpFont.Draw(rc);
}

void GameUI::Render(RenderContext& rc)
{
	DrawPlayerUI(rc);
	DrawMonsterUI(rc);
}

void GameUI::InitPlayerUI()
{
	//MP
	m_playerUI.m_mpFont.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_playerUI.m_mpFont.SetPosition(-800.0f, -300.0f);
	m_playerUI.m_mpFont.SetScale(1.5f);
	//HP
	m_playerUI.m_hpFont.SetColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_playerUI.m_hpFont.SetPosition(-800.0f, -200.0f);
	m_playerUI.m_hpFont.SetScale(1.5f);



}

void GameUI::InitMonsterUI()
{
	m_monsterUI.m_hpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_monsterUI.m_hpFont.SetScale(1.5f);
	m_monsterUI.m_hpFont.SetPosition(-800.0f, 500.0f);


}

