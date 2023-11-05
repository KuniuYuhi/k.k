#include "stdafx.h"
#include "Player.h"
#include "Wizard.h"
#include "Actor.h"
#include "Game.h"

#include "Brave.h"

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

	//現在のキャラクターを勇者に設定する
	m_nowActor = m_brave;

	//キャラクターの座標を設定
	m_nowActor->SetPosition(START_POSITION);

	//座標の設定
	m_position = m_nowActor->GetPosition();

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
		m_nowActor->ProcessWin();
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
	if (m_nowActor->GetDieFlag() == true)
	{
		return true;
	}
	return false;
}


int Player::GetAtk()
{
	return m_nowActor->GetStatus().atk;
}

bool Player::IsComboStateSame()
{
	return m_nowActor->IsComboStateSame();
}

void Player::SetDamagedComboState(Actor::EnComboState damagedcombostate)
{
	m_nowActor->SetDamagedComboState(damagedcombostate);
}

Actor::EnComboState Player::GetNowComboState() const
{
	return m_nowActor->GetNowComboState();
}



