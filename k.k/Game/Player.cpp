#include "stdafx.h"
#include "Player.h"
#include "Game.h"

#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"



namespace {
	const Vector3 START_POSITION = { 0.0f, 0.0f, -400.0f };
}

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
	m_brave->SetPosition(m_position);

	//キャラクターの座標を設定
	m_brave->SetPosition(START_POSITION);

	return true;
}

void Player::Update()
{

	if (IsInaction() == true)
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
	//勝敗が決まっていたら
	if (m_decisionOutComeFlag == true)
	{
		

		return true;
	}

	//////////////////////////////////////////////////
	// 行動出来なくなる条件
	////////////////////////////////////////////////// 

	//勝利したかつゲーム終了後の処理が終わったなら
	if (GameManager::GetInstance()->GetPlayerWinFlag()==true&&
		GameManager::GetInstance()->GetGameFinishProcessEndFlag())
	{
		//勝利時の処理実行
		m_brave->ProcessWin();
		//勝敗決定
		m_decisionOutComeFlag = true;
		return true;
	}
	//負けたら
	if (GameManager::GetInstance()->GetPlayerLoseFlag() == true)
	{
		//勝敗決定
		m_decisionOutComeFlag = true;
		//アイドル状態に遷移
		m_brave->ForciblyIdleAnim();
		//モデルが点滅しないようにする
		m_brave->SetInvicibleTimeFlag(false);
		return true;
	}


	//ゲームシーンステートでやる
	if (GameSceneManager::GetInstance()->GetCurrentGameSceneState() != enGameSceneState_Game)
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
	return m_brave->GetStatus().GetAtk();
}

int Player::GetSkillAtk()
{
	return m_brave->GetSkillAtk();
}

void Player::SetKnockBackInfo(
	bool flag, Vector3 endPos, float KnockBackPower, float knockBackLimmit)
{
	m_brave->SetHitKnockBackFlag(flag);
	m_brave->SettingKnockBackInfo(endPos, KnockBackPower, knockBackLimmit);

}

void Player::ProcessPlayerDead()
{
	//やられていないなら処理しない
	if (m_isPlayerDeadFlag != true)
	{
		return;
	}
}



