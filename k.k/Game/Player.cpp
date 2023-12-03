#include "stdafx.h"
#include "Player.h"
#include "Wizard.h"
#include "Actor.h"
#include "Game.h"

#include "CharactersInfoManager.h"

namespace {
	const Vector3 START_POSITION = { 0.0f, 0.0f, -400.0f };
}

//todo 重力
Player::Player()
{
}

Player::~Player()
{
	DeleteGO(m_brave);
}

bool Player::Start()
{
	//ゲームクラスと勇者クラスのインスタンスを検索
	m_game = FindGO<Game>("game");
	m_brave = NewGO<Brave>(0, "brave");
	//勇者インスタンスを代入
	CharactersInfoManager::GetInstance()->SetBraveInstance(m_brave);

	//キャラクターの座標を設定
	m_brave->SetPosition(START_POSITION);

	return true;
}

void Player::Update()
{
	//ゲームが始まるまでは移動しない
	if (m_game->GetNowGameState() != Game::enGameState_Game)
	{
		return;
	}
	//行動可能にする
	if (m_dontActionFlag != false)
	{
		SetDontActionFlag(false);
	}
}

bool Player::IsInaction()
{
	if (m_decisionOutComeFlag == true)
	{
		return true;
	}

	//////////////////////////////////////////////////
	// 行動出来なくなる条件
	////////////////////////////////////////////////// 
	//勝利したら 
	if (m_game->GetEnOutCome() == Game::enOutCome_Player_Win)
	{
		//勝利時の処理実行
		m_brave->ProcessWin();
		//
		m_decisionOutComeFlag = true;
		return true;
	}
	//負けたら
	if (m_game->GetEnOutCome() == Game::enOutCome_Player_Lose)
	{
		m_decisionOutComeFlag = true;
		return true;
	}
	//ゲーム中でないなら
	if (m_game->IsMatchGameState(Game::enGameState_Game) != true)
	{
		return true;
	}
	//////////////////////////////////////////////////
	// 行動できる
	//////////////////////////////////////////////////
	return false;
}

bool Player::IsDeadPlayer()
{
	//プレイヤーがやられたら
	if (m_brave->GetDieFlag() == true)
	{
		return true;
	}
	return false;
}

int Player::GetAtk()
{
	return m_brave->GetStatus().atk;
}

bool Player::IsComboStateSame()
{
	return m_brave->IsComboStateSame();
}

void Player::SetDamagedComboState(Actor::EnComboState damagedcombostate)
{
	m_brave->SetDamagedComboState(damagedcombostate);
}

Actor::EnComboState Player::GetNowComboState() const
{
	return m_brave->GetNowComboState();
}



