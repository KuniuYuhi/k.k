#include "stdafx.h"
#include "Player.h"
#include "Hero.h"
#include "Wizard.h"
#include "Actor.h"
#include "Game.h"

//todo ここで当たり判定するのもありかも
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
	m_game = FindGO<Game>("game");


	m_hero = NewGO<Hero>(0, "hero");
	m_wizard = NewGO<Wizard>(0, "wizard");


	actor[enHero] = m_hero;
	actor[enWizard] = m_wizard;

	
	//初期キャラクターをヒーローに設定
	m_enActiveCharacter = enHero;
	//サブのキャラクターをウィザードに設定
	m_subActor = actor[enWizard];

	//現在のキャラクターをヒーローに設定する
	m_nowActor = actor[m_enActiveCharacter];

	//キャラクターの座標を設定
	m_nowActor->SetPosition(m_position);

	//座標の設定
	m_position = m_nowActor->GetPosition();

	//キャラコンの設定
	m_charaCon.Init(12.0f, 33.0f, m_position);


	/*RigidBodyInitData rbid;
	rbid.collider = m_charaCon.GetCollider();
	rbid.mass = 20.0f;
	rbid.pos = m_position;
	rbid.restitution = 200.0f;
	rbid.rot = m_nowActor->GetRotation();

	rigitBody.Init(rbid);
	rigitBody.SetLinearFactor(1.0f, 0.0f, 1.0f);
	rigitBody.SetAngularFactor(0.0f, 1.0f, 0.0f);*/


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
	if (m_game->GetNowGameState() != Game::enGameState_Game)
	{
		return;
	}
	//行動可能にする
	if (m_dontActionFlag != false)
	{
		SetDontActionFlag(false);
	}

	//現在のキャラクターがやられたら強制的に切り替え
	if (ForcedChange() == false)
	{
		//フラグがfalseなら
		if (m_ChangCharacterFlag == false)
		{
			//キャラ切り替え
			Change();
		}
		
	}

	//キャラ切り替え後のクールタイムの計算
	ChangeCharacterTime();

	//現在のキャラクターがやられていないなら先の処理しない
	if (m_nowActor->GetDieFlag() != true)
	{
		//移動処理
		m_moveSpeed = m_nowActor->calcVelocity(m_nowActor->GetStatus());
		m_moveSpeed.y = 0.0f;
		
		CalcPosition(m_moveSpeed,1.0f/60.0f);
	}
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

	//m_charaCon.SetRadius(50.0f);
	//キャラコンの座標
	//m_nowActor->SetCharaConPosition(m_nowActor->GetPosition());
	//現在のキャラクターを魔法使いに変更する
	m_enActiveCharacter = nextCharacter;

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
	m_position = m_charaCon.Execute(moveSpeed, deltaTime);
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
	if (GameClear() == true)
	{
		return true;
	}
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



