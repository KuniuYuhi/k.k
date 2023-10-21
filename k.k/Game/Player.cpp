#include "stdafx.h"
#include "Player.h"
#include "Hero.h"
#include "Wizard.h"
#include "Actor.h"
#include "Game.h"

#include "Brave.h"

//todo 重力
Player::Player()
{
}

Player::~Player()
{
	DeleteGO(m_hero);
	DeleteGO(m_wizard);
}

bool Player::Start()
{
	//ゲームクラスと勇者クラスのインスタンスを検索
	m_game = FindGO<Game>("game");
	m_brave = NewGO<Brave>(0, "brave");

	actor[enHero] = m_hero;
	actor[enWizard] = m_wizard;

	actor[enBrave] = m_brave;
	
	//初期キャラクターをヒーローに設定
	m_enActiveCharacter = enBrave;
	//サブのキャラクターをウィザードに設定
	m_subActor = actor[enWizard];

	//現在のキャラクターをヒーローに設定する
	m_nowActor = actor[m_enActiveCharacter];

	//キャラクターの座標を設定
	m_nowActor->SetPosition(m_position);

	//座標の設定
	m_position = m_nowActor->GetPosition();

	return true;
}

void Player::Update()
{
	//勝敗が決まったら
	if (IsWinnerDecision() == true)
	{
		return;
	}

	//ゲームが始まるまでは移動しない
	//todo もっと分かりやすくできる
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
	//////////////////////////////////////////////////
	// 行動出来なくなる条件
	////////////////////////////////////////////////// 
	//ゲーム中でないなら
	if (m_game->IsMatchGameState(Game::enGameState_Game) != true)
	{
		return true;
	}
	//勝敗が決まったら
	if (IsWinnerDecision() == true)
	{
		return true;
	}

	//////////////////////////////////////////////////
	// 行動できる
	//////////////////////////////////////////////////
	return false;
}

void Player::Change()
{
	//サブのアクターがやられていたら先処理をしない
	if (m_subActor->GetDieFlag() == true)
	{
		return;
	}

	//特定のアニメーションが再生中の時は入れ替え可能
	if (m_nowActor->isAnimationSwappable() != true)
	{
		//キャラクターの切り替え
		if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			switch (m_enActiveCharacter)
			{
			case enHero:
				//切り替え先がやられていたら交代しない
				ChangeCharacter(enWizard);
				break;
			case enWizard:
				ChangeCharacter(enHero);
				break;

			default:
				break;
			}
			//キャラ切り替えフラグ
			SetChangCharacterFlag(true);
			//gameUIに知らせる用フラグ。gameUIでfalseに戻す
			SetChangCharacterFlagForGameUI(true);
		}
	}
}

void Player::ChangeCharacter(EnCharacters nextCharacter)
{
	//サブのアクターを入れ替える
	m_subActor = m_nowActor;

	// 現在のアクターを非アクティブにする。
	m_nowActor->Deactivate();

	// アクターを切り替える
	m_nowActor = actor[nextCharacter];

	//アクターのアニメーションを待機ステートにする
	//m_nowActor->SetAnimationState();

	m_nowActor->Activate();
	//座標、回転、サイズをヒーローと同じにする
	m_nowActor->SetTransForm(
		actor[m_enActiveCharacter]->GetPosition(),
		actor[m_enActiveCharacter]->GetRotation()
	);

	//キャラを切り替えたので無敵時間フラグをtrueにする
	m_nowActor->SetChangeCharacterInvincbleFlag(true);

	//現在のキャラクターを魔法使いに変更する
	m_enActiveCharacter = nextCharacter;

	//切り替え音再生
	g_soundManager->InitAndPlaySoundSource(enSoundName_ChangeCharacter, g_soundManager->GetSEVolume());
}

bool Player::ForcedChange()
{
	//現在のキャラクターがやられていないなら先の処理しない
	if (m_nowActor->GetDieFlag() != true)
	{
		return false;
	}

	//現在のキャラクターのDieアニメーションが終わり切り替え可能になったら
	if (m_nowActor->GetDieToChangeFlag() == true)
	{
		//数秒経ってから切り替え
		if (m_dieToChangeTime < m_dieToChangeTimer)
		{
			//タイマーリセット
			m_dieToChangeTimer = 0.0f;

			switch (m_enActiveCharacter)
			{
			case enHero:
				ChangeCharacter(enWizard);
				break;
			case enWizard:
				ChangeCharacter(enHero);
				break;

			default:
				break;
			}
			//gameUIに知らせる用フラグ。gameUIでfalseに戻す
			SetChangCharacterFlagForGameUI(true);
		}
		else
		{
			//タイマー加算
			m_dieToChangeTimer += g_gameTime->GetFrameDeltaTime();
		}

		
	}

	return true;
}

bool Player::ChangeCharacterTime()
{
	//trueでないなら処理しない
	if (m_ChangCharacterFlag == false)
	{
		return true;
	}

	if (0.0f >= m_ChangeCharacterTimer)
	{
		m_ChangCharacterFlag = false;
		m_ChangeCharacterTimer = m_ChangeCharacterTime;
		return true;
	}
	else
	{
		m_ChangeCharacterTimer -= g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

bool Player::IsAnnihilation()
{
	//現在のキャラクターとサブのキャラクターがやられていたら
	if (m_nowActor->GetDieToChangeFlag() == true && m_subActor->GetDieToChangeFlag() == true)
	{
		//全滅している
		return true;
	}
	//生存しているキャラクターがいる
	return false;
}

void Player::CalcPosition(Vector3 moveSpeed,float deltaTime)
{
}

int Player::GetAtk()
{
	return m_nowActor->GetStatus().atk;
}

int Player::GetSkillAtk()
{
	return m_nowActor->GetSkillAttackPower();
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

bool Player::IsWinnerDecision()
{
	//勝利
	//ボスが死んだら処理しない
	/*if (GameClear() == true)
	{
		return true;
	}*/
	//敗北
	//キャラクターが全滅していたら処理しない
	if (GameOver() == true)
	{
		return true;
	}
	//タイムアップ
	if (TimeUp() == true)
	{
		//ここかく
		return true;
	}
	//それ以外なら	
	return false;
}

bool Player::GameClear()
{
	if (m_game->GetDeathBossFlag() == true)
	{
		m_gameEndFlag = true;

		if (m_VictoryAnimFlag == false)
		{
			//アニメーションしていなかったら
			if (m_nowActor->isAnimationEntable() == true)
			{
				//現在のアクターのステートを勝利ステートにする
				m_nowActor->SetVictoryAnimationState();
				m_VictoryAnimFlag = true;
			}
		}
		return true;
	}

	return false;
}

bool Player::GameOver()
{
	if (IsAnnihilation() == true)
	{
		m_gameEndFlag = true;
		//ゲームに全滅したことを一度だけ伝える
		if (m_informGameFlag != true)
		{
			Game* game = FindGO<Game>("game");
			game->SetPlayerAnnihilationFlag(true);
			m_informGameFlag = true;
		}

		return true;
	}

	return false;
}

bool Player::TimeUp()
{
	if (m_game->IsTimeUp() == true)
	{
		m_gameEndFlag = true;

		if (m_idleAnimFlag == false)
		{
			//現在のアクターのステートをアイドルステートにする
			m_nowActor->SetIdleAnimationState();
			m_idleAnimFlag = true;
			
		}
		return true;
	}

	return false;
}



